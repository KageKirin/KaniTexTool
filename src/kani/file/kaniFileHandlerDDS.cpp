//
//  kaniFileHandlerDDS.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 10.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#include "kaniFileHandlerDDS.h"

#include "../core/kaniTypes.h"
#include "../core/kaniEndian.h"

#include <pvrtex/CPVRTexture.h>
#include <pvrtex/CPVRTextureHeader.h>
#include <pvrtex/CPVRTextureData.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

//--------------------------------------------
//DDS file defines

//  DDS_header.dwFlags
#define DDSD_CAPS					0x00000001 
#define DDSD_HEIGHT					0x00000002 
#define DDSD_WIDTH					0x00000004 
#define DDSD_PITCH					0x00000008 
#define DDSD_PIXELFORMAT			0x00001000 
#define DDSD_MIPMAPCOUNT			0x00020000 
#define DDSD_LINEARSIZE				0x00080000 
#define DDSD_DEPTH					0x00800000 

//  DDS_header.sPixelFormat.dwFlags
#define DDPF_ALPHAPIXELS			0x00000001 
#define DDPF_FOURCC					0x00000004 
#define DDPF_INDEXED				0x00000020 
#define DDPF_RGB					0x00000040 

//  DDS_header.sCaps.dwCaps1
#define DDSCAPS_COMPLEX				0x00000008 
#define DDSCAPS_TEXTURE				0x00001000 
#define DDSCAPS_MIPMAP				0x00400000 

//  DDS_header.sCaps.dwCaps2
#define DDSCAPS2_CUBEMAP			0x00000200 
#define DDSCAPS2_CUBEMAP_POSITIVEX	0x00000400 
#define DDSCAPS2_CUBEMAP_NEGATIVEX	0x00000800 
#define DDSCAPS2_CUBEMAP_POSITIVEY	0x00001000 
#define DDSCAPS2_CUBEMAP_NEGATIVEY	0x00002000 
#define DDSCAPS2_CUBEMAP_POSITIVEZ	0x00004000 
#define DDSCAPS2_CUBEMAP_NEGATIVEZ	0x00008000 
#define DDSCAPS2_VOLUME				0x00200000 


namespace dds
{
	using namespace kani;
	using namespace kani::endian;

	static const uint32 DDS_Sign = static_swap<uint32>::swap<uint32('DDS\0')>::value;
	
	static const uint32 FourCC_DXT1 = static_swap<uint32>::swap<uint32('DXT1')>::value;
	static const uint32 FourCC_DXT2 = static_swap<uint32>::swap<uint32('DXT2')>::value;
	static const uint32 FourCC_DXT3 = static_swap<uint32>::swap<uint32('DXT3')>::value;
	static const uint32 FourCC_DXT4 = static_swap<uint32>::swap<uint32('DXT4')>::value;
	static const uint32 FourCC_DXT5 = static_swap<uint32>::swap<uint32('DXT5')>::value;
	
	//non-standard fourcc's
	static const uint32 FourCC_PVR2 = static_swap<uint32>::swap<uint32('PVR2')>::value;
	static const uint32 FourCC_PVR4 = static_swap<uint32>::swap<uint32('PVR4')>::value;
	static const uint32 FourCC_ETC1 = static_swap<uint32>::swap<uint32('ETC1')>::value;
	
	
	struct DDS_Pixelformat
	{
		uint32	dwSize;
		uint32	dwFlags;
		uint32	dwFourCC;
		uint32	dwRGBBitCount;
		uint32	dwRBitMask;
		uint32	dwGBitMask;
		uint32	dwBBitMask;
		uint32	dwABitMask;
	};
	
	struct DDS_Caps
	{
		uint32	dwCaps1;
		uint32	dwCaps2;
		uint32	dwDDSX;
		uint32	dwReserved;
    };
	
	union DDS_Header 
	{
		struct
		{
			uint32	dwSign;
			uint32	dwSize;
			uint32	dwFlags;
			uint32	dwHeight;
			uint32	dwWidth;
			uint32	dwPitchOrLinearSize;
			uint32	dwDepth;
			uint32	dwMipMapCount;
			uint32	dwReserved1[11];
			DDS_Pixelformat	ddspf;
			DDS_Caps		ddsc;
			uint32	dwReserved2;
		};
		char	data[sizeof(128)];
	};
}




#define DDS_MAGIC_FLIPPED     0x0F7166ED

//--------------------------------------------
// DDS file format structures.

namespace kani { namespace file {

	using std::cout;
	using std::cerr;
	using std::endl;
	using std::ifstream;
	using std::ofstream;
	using std::ios_base;
	using std::vector;
	using dds::DDS_Header;
	
	//ref: @see http://www.codesampler.com/oglsrc/oglsrc_4.htm
	//ref: @see http://www.racer.nl/tech/dds.htm
	//ref: @see http://www.mindcontrol.org/~hplus/graphics/dds-info/

	template<>
	int FileHandlerImpl<FileType_DDS>::internal_read(const string& filename, CPVRTextureHeader& pvrHeader, CPVRTextureData& pvrData) const
	{
		DDS_Header ddsHeader;
		memset(&ddsHeader, 0, sizeof(DDS_Header));
		
		ifstream file(filename.c_str(), ios_base::in | ios_base::binary);
		assert(file.is_open());
		if(!file.is_open())
			return -2;
		
		file.read(ddsHeader.data, sizeof(DDS_Header));
		
		assert(ddsHeader.dwSign == dds::DDS_Sign);
		if(ddsHeader.dwSign != dds::DDS_Sign)
			return -3;	//not a DDS file

		assert(ddsHeader.dwSize == sizeof(DDS_Header) - sizeof(uint32));
		if(ddsHeader.dwSize != sizeof(DDS_Header) - sizeof(uint32))
			return -3;	//not a DDS file
		
		assert(ddsHeader.dwFlags & DDSD_PIXELFORMAT);
		if(!(ddsHeader.dwFlags & DDSD_PIXELFORMAT))
			return -3;
		
		assert(ddsHeader.dwFlags & DDSD_CAPS);
		if(!(ddsHeader.dwFlags & DDSD_CAPS))
			return -3;
		
		
		pvrHeader = CPVRTextureHeader(ddsHeader.dwWidth, ddsHeader.dwHeight);
		
		if(ddsHeader.ddspf.dwFlags & DDPF_FOURCC)
		{
			//TODO: use our enum for this, extend to contain (FourCC or 0)
			//TODO: create another enum to compute data size depending on FourCC
			switch(ddsHeader.ddspf.dwFourCC)
			{
				case dds::FourCC_DXT1:
					break;

				case dds::FourCC_DXT2:
					break;
					
				case dds::FourCC_DXT3:
					break;
					
				case dds::FourCC_DXT4:
					break;
					
				case dds::FourCC_DXT5:
					break;
					
				case dds::FourCC_PVR2:
					break;
					
				case dds::FourCC_PVR4:
					break;
					
				case dds::FourCC_ETC1:
					break;
					
				//extend for more fourCCs
			}
		}
		else if()
		{
			
		}
		
		//compute size
		size_t datasize = 0;
		for(uint32 i = 0; i < ddsHeader.dwMipMapCount; ++i)
		{
			uint32 mipWidth		= ddsHeader.dwWidth		>> i;
			uint32 mipHeight	= ddsHeader.dwHeight	>> i;
			uint32 mipDepth		= ddsHeader.dwDepth		>> i;
			//a bit more: number of surfaces, etc...
			datasize += format.computeSize(mipWidth, mipHeight, mipDepth);
		}
		
		//read data
		vector<char>	data(datasize, 0);
		file.read(&data[0], datasize);
		
		int readBytes = (int)file.tellg();
		file.close();
		
		pvrData = CPVRTextureData((pvrtexlib::uint8*)&data[0], datasize);
		
		return readBytes;
	}
	
	template<>
	int FileHandlerImpl<FileType_DDS>::internal_write(const string& filename, const CPVRTextureHeader& pvrHeader, const CPVRTextureData& pvrData) const
	{
		return -5;
	}

	
}}



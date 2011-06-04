//
//  kaniFileHandlerDDS.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 10.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

//#include "stdafx.h"
#include "kaniFileHandlerDDS.h"

#include "../core/kaniTypes.h"
#include "../core/kaniEndian.h"
#include "../texture/kaniTexFourCC.h"
#include "../texture/kaniTexSize.h"
#include "../texture/kaniTexFormatTuple.h"
#include "../texture/kaniTexPixelFormat.h"

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
	using namespace kani::texture;
	using kani::texture::TextureSize;
	using kani::texture::TexFormatTuple;
		
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
		
		
		pvrHeader = CPVRTextureHeader((ddsHeader.dwFlags & DDSD_WIDTH)	? ddsHeader.dwWidth : 1, 
									  (ddsHeader.dwFlags & DDSD_HEIGHT)	? ddsHeader.dwHeight : 1);

		uint32 mips = (ddsHeader.dwFlags & DDSD_MIPMAPCOUNT) ? ddsHeader.dwMipMapCount : 1;
		pvrHeader.setMipMapCount(mips);
		
		PixelType pt = (ddsHeader.ddspf.dwFlags & DDPF_FOURCC) ? 
			getSupportedPixelType(ddsHeader.ddspf.dwFourCC) :
			getPixelTypeForMask(ddsHeader.ddspf.dwRBitMask, ddsHeader.ddspf.dwGBitMask, ddsHeader.ddspf.dwBBitMask, ddsHeader.ddspf.dwABitMask);
		pvrHeader.setPixelType(pt);
		
		pvrHeader.setVolume(ddsHeader.ddsc.dwCaps2  & DDSCAPS2_VOLUME);
		pvrHeader.setCubeMap(ddsHeader.ddsc.dwCaps2 & DDSCAPS2_CUBEMAP);

		assert(!(ddsHeader.ddspf.dwFlags & DDPF_INDEXED));	//we don't support INDEXED atm
		const TextureSize& textureSize = (ddsHeader.ddspf.dwFlags & DDPF_FOURCC) ?
			TextureSize::getTextureSize((texture::FourCC)ddsHeader.ddspf.dwFourCC) :
			(ddsHeader.ddspf.dwFlags & (DDPF_RGB | DDPF_ALPHAPIXELS)) ?
			 TextureSize::getTextureSize(4, ddsHeader.ddspf.dwRGBBitCount) :
			 TextureSize::getTextureSize(3, ddsHeader.ddspf.dwRGBBitCount);
		
		//compute size
		size_t datasize = 0;
		int depth = (ddsHeader.dwFlags & DDSD_DEPTH) ? ddsHeader.dwDepth : 1;
		for(uint32 i = 0; i < mips; ++i)
		{
			//a bit more: number of surfaces, etc...
			datasize += textureSize(ddsHeader.dwWidth, ddsHeader.dwHeight, depth, i, false);
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
		//fill DDS header from PVR header
		DDS_Header ddsHeader;
		memset(&ddsHeader, 0, sizeof(DDS_Header));
		ddsHeader.dwSign = dds::DDS_Sign;
		ddsHeader.dwSign = sizeof(DDS_Header);
		ddsHeader.ddspf.dwSize = sizeof(dds::DDS_Pixelformat);
		ddsHeader.dwFlags = DDSD_PIXELFORMAT & DDSD_CAPS;
		
		ddsHeader.dwWidth = pvrHeader.getWidth();
		ddsHeader.dwFlags &= DDSD_WIDTH;
		
		ddsHeader.dwHeight = pvrHeader.getHeight();
		ddsHeader.dwFlags &= DDSD_HEIGHT;
		
		ddsHeader.dwDepth = pvrHeader.getNumSurfaces();
		ddsHeader.dwMipMapCount = pvrHeader.getMipMapCount();
		if(ddsHeader.dwMipMapCount)
			ddsHeader.dwFlags &= DDSD_MIPMAPCOUNT;
		if(pvrHeader.isVolume())
			ddsHeader.ddsc.dwCaps2 &= DDSCAPS2_VOLUME;
		if(pvrHeader.isCubeMap())
			ddsHeader.ddsc.dwCaps2 &= DDSCAPS2_CUBEMAP;

		
		pvrtexlib::PixelType pt = pvrHeader.getPixelType();
		const TexFormatTuple& tfTuple = texture::getTexFormatTuple(pt);
		
		if(tfTuple.fourCC)
		{
			ddsHeader.ddspf.dwFlags &= DDPF_FOURCC;
			ddsHeader.ddspf.dwFourCC = tfTuple.fourCC;
		}
		else 
		{
			ddsHeader.ddspf.dwFlags &= DDPF_RGB;
			if(pvrHeader.hasAlpha())
			{
				ddsHeader.ddspf.dwFlags &= DDPF_ALPHAPIXELS;			
			}

			//set color mask
			getMaskForPixelType(tfTuple.pvrtex, 
								&ddsHeader.ddspf.dwRBitMask, &ddsHeader.ddspf.dwGBitMask, 
								&ddsHeader.ddspf.dwBBitMask, &ddsHeader.ddspf.dwABitMask);
		}
		
		size_t dataSize = pvrData.getDataSize();
		
		//write
		ofstream file(filename.c_str(), ios_base::out | ios_base::binary);
		assert(file.is_open());
		if(!file.is_open())
			return -2;
		
		file.write(ddsHeader.data, sizeof(DDS_Header));
		file.write((char*)pvrData.getData(), dataSize);
		
		int writtenBytes = (int)(sizeof(DDS_Header) + dataSize);
		file.close();
		
		return writtenBytes;
	}

	
}}



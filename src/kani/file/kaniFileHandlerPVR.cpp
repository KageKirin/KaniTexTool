//
//  kaniFileHandlerPVR.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 10.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#include "stdafx.h"
#include "kaniFileHandlerPVR.h"
#include <pvrtex/CPVRTexture.h>
#include <pvrtex/CPVRTextureHeader.h>
#include <pvrtex/CPVRTextureData.h>
#include <fstream>
#include <cstdio>
#include <vector>

namespace kani { namespace file {
	
	using std::ifstream;
	using std::ofstream;
	using std::ios_base;
	using std::vector;
	
		//TODO: use vector<png_byte*> rowPtrs instead of current array
	//TODO: use for data as well and stop worrying about memory leaks
	
	struct _PVRTexHeader
	{
		uint32_t headerLength;
		uint32_t height;
		uint32_t width;
		uint32_t numMipmaps;
		uint32_t flags;
		uint32_t dataLength;
		uint32_t bpp;
		uint32_t bitmaskRed;
		uint32_t bitmaskGreen;
		uint32_t bitmaskBlue;
		uint32_t bitmaskAlpha;
		uint32_t pvrTag;
		uint32_t numSurfs;
	};
	
	//CPVRTexture 
	
	template<>
	int FileHandlerImpl<FileType_PVR>::internal_read(const string& filename, CPVRTextureHeader& header, CPVRTextureData& data) const
	{
		ifstream file(filename.c_str(), ios_base::in | ios_base::binary);
		if(!file.is_open())
			return -2;
			
		file.seekg(0, ios_base::end);
		size_t fileSize = file.tellg();
		file.seekg (0, ios_base::beg);
		
		vector<char> buffer(fileSize, 0);
		file.read((char*)&buffer[0], fileSize);
		
		int readBytes = (int)file.tellg();
		file.close();
		
		{
			pvrtexlib::CPVRTexture tex((pvrtexlib::uint8*)&buffer[0]);
			header	= CPVRTextureHeader(tex.getHeader());		
			data	= CPVRTextureData(tex.getData());
		}		
		return readBytes;
	}
	
	template<>
	int FileHandlerImpl<FileType_PVR>::internal_write(const string& filename, const CPVRTextureHeader& header, const CPVRTextureData& data) const
	{
		FILE* file = fopen(filename.c_str(), "w+b");
		if(!file)
			return -2;
		
		int writtenBytes = 0;
		writtenBytes += header.writeToFile(file);
		writtenBytes += data.writeToFile(file);
		
		fclose(file);
		
		return writtenBytes;
	}
		
}}

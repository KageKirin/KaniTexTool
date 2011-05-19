//
//  kaniFileTypes.h
//  KaniTexTool
//
//  Created by Christian Helmich on 10.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#pragma once
#ifndef KANI_FILE_TYPES_H
#define KANI_FILE_TYPES_H 1

#include "../core/kaniEndian.h"
#include <string>
#include <algorithm>

namespace kani { namespace file {
	
	using std::string;
	using std::transform;
	
	enum FileType
	{
		FileType_Unknown = -1,
		FileType_DDS = endian::static_swap<uint32>::swap<uint32('DDS\0')>::value,
		FileType_KTX = endian::static_swap<uint32>::swap<uint32('KTX\0')>::value,
		FileType_PVR = endian::static_swap<uint32>::swap<uint32('PVR\0')>::value,
		FileType_PNG = endian::static_swap<uint32>::swap<uint32('PNG\0')>::value,
	};
	
	//TODO: move to cpp
	//TODO: C++11 use && to return fileExt
	string		getFileExtFromFilename(const string& filename)
	{
		string fileExt = filename.substr(filename.find_last_of('.')+1);
		transform(fileExt.begin(), fileExt.end(),
				   fileExt.begin(), tolower);
		return fileExt;
	}
	
	FileType	getFileTypeForExt(const string& fileExt)
	{
		//TODO: use map instead
		if(fileExt == "dds")
			return FileType_DDS;
		if(fileExt == "ktx")
			return FileType_KTX;
		if(fileExt == "pvr")
			return FileType_PVR;
		if(fileExt == "png")
			return FileType_PNG;
		return FileType_Unknown;
	}
	
	
	FileType	getFileTypeFromFilename(const string& filename)
	{
		string fileExt = getFileExtFromFilename(filename);
		return getFileTypeForExt(fileExt);
	}
	
	
}}

#endif	//KANI_FILE_TYPES_H

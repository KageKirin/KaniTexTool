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

namespace kani { namespace filetypes {
	
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
	
	FileType	getFileTypeFromFilename(const string& filename)
	{
		string filetype = filename.substr(filename.find_last_of('.')+1);
		transform(filetype.begin(), filetype.end(),
				   filetype.begin(), tolower);
	
		if(filetype == "dds")
			return FileType_DDS;
		if(filetype == "ktx")
			return FileType_KTX;
		if(filetype == "pvr")
			return FileType_PVR;
		if(filetype == "png")
			return FileType_PNG;
		return FileType_Unknown;
	}
}}

#endif	//KANI_FILE_TYPES_H

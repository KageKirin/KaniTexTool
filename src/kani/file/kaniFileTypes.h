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


namespace kani { namespace file {
	
	using std::string;
	
	enum FileType
	{
		FileType_Unknown = -1,
		FileType_DDS = endian::static_swap<uint32>::swap<uint32('DDS\0')>::value,
		FileType_KTX = endian::static_swap<uint32>::swap<uint32('KTX\0')>::value,
		FileType_PVR = endian::static_swap<uint32>::swap<uint32('PVR\0')>::value,
		FileType_PNG = endian::static_swap<uint32>::swap<uint32('PNG\0')>::value,
	};
	
	string		getFileExtFromFilename(const string& filename);		//TODO: C++11 use && to return fileExt
	FileType	getFileTypeForExt(const string& fileExt);	
	FileType	getFileTypeFromFilename(const string& filename);	
	
}}

#endif	//KANI_FILE_TYPES_H

//
//  kaniFileHandlerPNG.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 10.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#include "kaniFileHandlerPNG.h"

namespace kani { namespace file {
	
	template<>
	int FileHandlerImpl<FileType_PNG>::internal_read(const string& filename, CPVRTextureHeader&, CPVRTextureData&) const
	{
		return -3;
	}
	
	template<>
	int FileHandlerImpl<FileType_PNG>::internal_write(const string& filename, const CPVRTextureHeader&, const CPVRTextureData&) const
	{
		return -3;
	}

	
}}

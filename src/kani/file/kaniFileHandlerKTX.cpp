//
//  kaniFileHandlerKTX.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 10.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

//#include "stdafx.h"
#include "kaniFileHandlerKTX.h"


namespace kani { namespace file {
	
	template<>
	int FileHandlerImpl<FileType_KTX>::internal_read(const string& filename, CPVRTextureHeader&, CPVRTextureData&) const
	{
		return -4;
	}
	
	template<>
	int FileHandlerImpl<FileType_KTX>::internal_write(const string& filename, const CPVRTextureHeader&, const CPVRTextureData&) const
	{
		return -4;
	}
	
}}


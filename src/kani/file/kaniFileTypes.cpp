//
//  kaniFileTypes.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 19.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

//#include "stdafx.h"
#include "kaniFileTypes.h"
#include <algorithm>

namespace kani { namespace file {

	using std::transform;


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

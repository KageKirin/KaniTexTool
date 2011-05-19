//
//  kaniFileHandler.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 18.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#include "kaniFileHandler.h"

namespace kani { namespace file {

	//TODO refactor to use Action template
	
	int FileHandler::read(const string& filename, CPVRTextureHeader& hdr, CPVRTextureData& data)
	{
		FileType fileType = getFileTypeFromFilename(filename);
		FileHandler& fileHandler = getFileHandlerForType(fileType);
		return fileHandler.internal_read(filename, hdr, data);
	}
	
	int FileHandler::write(const string& filename, const CPVRTextureHeader& hdr, const CPVRTextureData& data)
	{
		FileType fileType = getFileTypeFromFilename(filename);
		FileHandler& fileHandler = getFileHandlerForType(fileType);
		return fileHandler.internal_write(filename, hdr, data);
	}
	
	
	
	FileHandler& FileHandler::getFileHandlerForExt(const string& ext)
	{
		FileType fileType = getFileTypeForExt(ext);
		return getFileHandlerForType(fileType);
	}
	
	FileHandler& FileHandler::getFileHandlerForType(FileType filetype)
	{
	}
	
	
	
	int FileHandler::internal_read(const string& filename, CPVRTextureHeader&, CPVRTextureData&) const
	{
		return -1;
	}
	
	int FileHandler::internal_write(const string& filename, const CPVRTextureHeader&, const CPVRTextureData&) const
	{
		return -1;
	}


}}


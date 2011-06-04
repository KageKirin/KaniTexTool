//
//  kaniFileHandler.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 18.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

//#include "stdafx.h"
#include "kaniFileHandler.h"
#include "kaniFileHandlerImpl.h"
#include "kaniFileHandlerDDS.h"
#include "kaniFileHandlerKTX.h"
#include "kaniFileHandlerPNG.h"
#include "kaniFileHandlerPVR.h"

#include <map>

namespace kani { namespace file {

	using std::map;
	
	//----------------------------------------------------------------------
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

	//----------------------------------------------------------------------	
	
	int FileHandler::internal_read(const string& filename, CPVRTextureHeader&, CPVRTextureData&) const
	{
		return -1;
	}
	
	int FileHandler::internal_write(const string& filename, const CPVRTextureHeader&, const CPVRTextureData&) const
	{
		return -1;
	}

	//----------------------------------------------------------------------	
	
#define REGISTER_FILEHANDLERIMPL(map, filetype)	\
	static FileHandlerImpl<filetype>	s_FileHandler_ ##filetype;	\
	map[filetype] = & s_FileHandler_ ##filetype; 
	
	typedef map<FileType, FileHandler*>		FileHandlerMap;
	static FileHandlerMap	s_FileHandlerMap;
	static FileHandler		s_FileHandlerBase;
	
	static void createFileHandlerMap()
	{
		static bool s_created = false;
		if(s_created)
			return;
			
		s_FileHandlerMap.clear();
		
		REGISTER_FILEHANDLERIMPL(s_FileHandlerMap, FileType_DDS);
		REGISTER_FILEHANDLERIMPL(s_FileHandlerMap, FileType_KTX);
		REGISTER_FILEHANDLERIMPL(s_FileHandlerMap, FileType_PNG);
		REGISTER_FILEHANDLERIMPL(s_FileHandlerMap, FileType_PVR);
		
		s_created = true;
	}
	
	
	FileHandler& FileHandler::getFileHandlerForExt(const string& ext) 
	{
		FileType fileType = getFileTypeForExt(ext);
		return getFileHandlerForType(fileType);
	}
	
			
	FileHandler& FileHandler::getFileHandlerForType(FileType filetype)
	{
		createFileHandlerMap();

		FileHandlerMap::iterator iter = s_FileHandlerMap.find(filetype);
		if(iter != s_FileHandlerMap.end())
			return *(iter->second);
		
		return s_FileHandlerBase;
	}

	//----------------------------------------------------------------------
}}


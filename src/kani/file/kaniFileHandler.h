//
//  kaniFileHandler.h
//  KaniTexTool
//
//  Created by Christian Helmich on 18.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#pragma once
#ifndef KANI_FILE_HANDLER_H
#define KANI_FILE_HANDLER_H 1

#include "kaniFileTypes.h"
#include <string>

namespace pvrtexlib
{
	class CPVRTextureHeader;
	class CPVRTextureData;
}

namespace kani { namespace file {

	using pvrtexlib::CPVRTextureHeader;
	using pvrtexlib::CPVRTextureData;
	
	using std::string;
	
	//NOTE: using PVR structures as a common denominator, since we use PRTexLib to handle the compression.
	//TODO later: use a similar data structure from my own
	
	class FileHandler
	{
	public:
		static int read(const string& filename, CPVRTextureHeader&, CPVRTextureData&);
		static int write(const string& filename, const CPVRTextureHeader&, const CPVRTextureData&);
	
	protected:
		static FileHandler& getFileHandlerForExt(const string& ext);
		static FileHandler& getFileHandlerForType(FileType filetype);
		virtual int internal_read(const string& filename, CPVRTextureHeader&, CPVRTextureData&) const;
		virtual int internal_write(const string& filename, const CPVRTextureHeader&, const CPVRTextureData&) const;
	};	
	
}}


#endif	//KANI_FILE_HANDLER_H

//
//  kaniFileHandlerImpl.h
//  KaniTexTool
//
//  Created by Christian Helmich on 19.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#pragma once
#ifndef KANI_FILE_HANDLER_IMPL_H
#define KANI_FILE_HANDLER_IMPL_H	1

#include "kaniFileTypes.h"
#include "kaniFileHandler.h"

namespace kani { namespace file {

	template<FileType T>
	class FileHandlerImpl : public FileHandler
	{
	protected:
		virtual int internal_read(const string& filename, CPVRTextureHeader&, CPVRTextureData&) const;
		virtual int internal_write(const string& filename, const CPVRTextureHeader&, const CPVRTextureData&) const;
	};

}}

#endif	//KANI_FILE_HANDLER_IMPL_H

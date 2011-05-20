//
//  kaniFileHandlerKTX.h
//  KaniTexTool
//
//  Created by Christian Helmich on 10.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#pragma once
#ifndef KANI_FILE_HANDLER_KTX_H
#define KANI_FILE_HANDLER_KTX_H 1

#include "kaniFileTypes.h"
#include "kaniFileHandlerImpl.h"

namespace kani { namespace file {
	
	template<>
	int FileHandlerImpl<FileType_KTX>::internal_read(const string& filename, CPVRTextureHeader&, CPVRTextureData&) const;
	
	template<>
	int FileHandlerImpl<FileType_KTX>::internal_write(const string& filename, const CPVRTextureHeader&, const CPVRTextureData&) const;
	
}}

#endif	//KANI_FILE_HANDLER_KTX_H
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

#include "../core/kaniUtils.h"

namespace kani { namespace filetypes {

	enum FileTypes
	{
		FileUnknown = -1,
		FileDDS = endian::static_swap<uint32>::swap<uint32('DDS\0')>::value,
		FileKTX = endian::static_swap<uint32>::swap<uint32('KTX\0')>::value,
		FilePVR = endian::static_swap<uint32>::swap<uint32('PVR\0')>::value,
		FilePNG = endian::static_swap<uint32>::swap<uint32('PNG\0')>::value,
	};
	
}}

#endif	//KANI_FILE_TYPES_H

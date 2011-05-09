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
		FileDDS = static_swap<uint32>::swap<('DDS\0')>::value,
		FileKTX = static_swap<uint32>::swap<('KTX\0')>::value,
		FilePVR = static_swap<uint32>::swap<('PVR\0')>::value,
		FilePNG = static_swap<uint32>::swap<('PNG\0')>::value,
	};
	
}}

#endif	//KANI_FILE_TYPES_H

//
//  kaniTexFourCC.h
//  KaniTexTool
//
//  Created by Christian Helmich on 6/1/11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#pragma once
#ifndef KANI_TEX_FOURCC_H
#define	KANI_TEX_FOURCC_H	1

#include "../core/kaniTypes.h"
#include "../core/kaniEndian.h"

namespace kani { namespace texture {
	
	using kani::uint32;
	using kani::endian::static_swap;

	//dds standard fourcc's
	static const uint32 FourCC_DXT1 = static_swap<uint32>::swap<uint32('DXT1')>::value;
	static const uint32 FourCC_DXT2 = static_swap<uint32>::swap<uint32('DXT2')>::value;
	static const uint32 FourCC_DXT3 = static_swap<uint32>::swap<uint32('DXT3')>::value;
	static const uint32 FourCC_DXT4 = static_swap<uint32>::swap<uint32('DXT4')>::value;
	static const uint32 FourCC_DXT5 = static_swap<uint32>::swap<uint32('DXT5')>::value;
	
	//non-standard fourcc's
	static const uint32 FourCC_PVR2 = static_swap<uint32>::swap<uint32('PVR2')>::value;
	static const uint32 FourCC_PVR4 = static_swap<uint32>::swap<uint32('PVR4')>::value;
	static const uint32 FourCC_ETC1 = static_swap<uint32>::swap<uint32('ETC1')>::value;
	
	//extend here
	
}}

#endif	//KANI_TEX_FOURCC_H
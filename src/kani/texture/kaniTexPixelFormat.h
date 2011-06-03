//
//  kaniTexPixelFormat.h
//  KaniTexTool
//
//  Created by Christian Helmich on 5/30/11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#pragma once
#ifndef KANI_TEX_PIXEL_TYPE
#define KANI_TEX_PIXEL_TYPE	1

#include "../core/kaniTypes.h"
#include <pvrtex/PVRTexLibGlobals.h>


namespace kani { namespace texture {
	
	using pvrtexlib::PixelType;
	using kani::uint32;
	struct TexFormatTuple;

	const TexFormatTuple& getTexFormatTuple(const char* textFormat);
	const TexFormatTuple& getTexFormatTuple(int pngFormat, int bitPerChannel);
	const TexFormatTuple& getTexFormatTuple(PixelType pixelType);
	const TexFormatTuple& getTexFormatTuple(uint32 order);
			
	PixelType	getSupportedPixelType(const char* textFormat);
	PixelType	getSupportedPixelType(int pngFormat, int bitPerChannel);
	PixelType	getSupportedPixelType(uint32 fourCC);
	
	PixelType	getPixelTypeForMask(uint32 rMask, uint32 gMask, uint32 bMask, uint32 aMask);
	void		getMaskForPixelType(PixelType pt, uint32* rMask, uint32* gMask, uint32* bMask, uint32* aMask);
	
	struct PngFormatInfo
	{
		int	colorType;
		int bits;
	};
	
	PngFormatInfo	getPngFormatInfo(PixelType pixelType);

}}

#endif	//KANI_TEX_PIXEL_TYPE
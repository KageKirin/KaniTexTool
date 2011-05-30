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

#include <pvrtex/PVRTexLibGlobals.h>


namespace kani { namespace texture {
	
	using pvrtexlib::PixelType;

	
	PixelType	getSupportedPixelType(const char* textFormat);
	PixelType	getSupportedPixelType(int pngFormat, int bitPerChannel);
	
	struct PngFormatInfo
	{
		int	colorType;
		int bits;
	};
	
	PngFormatInfo	getPngFormatInfo(PixelType pixelType);

}}

#endif	//KANI_TEX_PIXEL_TYPE
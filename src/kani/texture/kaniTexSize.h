//
//  kaniTexSize.h
//  KaniTexTool
//
//  Created by Christian Helmich on 6/1/11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#pragma once
#ifndef KANI_TEX_SIZE_H
#define KANI_TEX_SIZE_H	1

#include "../core/kaniTypes.h"
#include "kaniTexFourCC.h"

namespace kani { namespace texture {
	
	struct	TextureSize
	{
		virtual	int	operator()(int width, int height, int mipLevel = 0, bool padding = false) const;
		virtual	int	operator()(int width, int height, int depth, int mipLevel = 0, bool padding = false) const;
		
		static const TextureSize& getTextureSize(FourCC fourCC);
		static const TextureSize& getTextureSize(int channels, int bits);
	};	
	
}}

#endif	//KANI_TEX_SIZE_H

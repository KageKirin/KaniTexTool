//
//  kaniTexFormat.h
//  KaniTexTool
//
//  Created by Christian Helmich on 15.05.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once
#ifndef KANI_TEX_FORMAT
#define KANI_TEX_FORMAT	1

#include <pvrtex/PVRTexLibGlobals.h>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

namespace kani { namespace texture {

	using pvrtexlib::PixelType;


#define	FORMAT_SEQ	\
	(SameAsInput)	\
	(A4R4G4B4)		\
	(A1R5G5B5)		\
	(R5G6B5)		\
	(R5G5B5)		\
	(A8R8G8B8)		\
	(A8R3G3B3)		\
	(I8)			\
	(A8)			\
	(A8I8)			\
	(BITMAP)		\
	(VY1UY0)		\
	(Y1VY0U)		\
	(R4G44BA4)		\
	(R5G5B5A1)		\
	(R8G8B8A8)		\
	(R8G8B8)		\
	(B8G8R8A8)		\
	(PVRTC2)		\
	(PVRTC4)		\
	(DXT1)			\
	(DXT2)			\
	(DXT3)			\
	(DXT4)			\
	(DXT5)			\
	(ETC)			\
	(R3G3B2)		\
	(A10B10G10R10)	\
	(A10R10G10B10)	\
	(G16R16)		\
	(V16U16)		\
	(A16B16G16R16)	\
	(R16F)			\
	(GR16F)			\
	(ABGR16F)		\
	(R32F)			\
	(GR32F)			\
	(ABGR32F)		
	

#define M_CAT(r, data, elem) BOOST_PP_CAT(data, elem),
		
	enum Format
	{
		BOOST_PP_SEQ_FOR_EACH(M_CAT, Format_, FORMAT_SEQ) // Format_x
		Format_Count		
	};
#undef M_CAT	

	PixelType	getSupportedPixelType(const char* textFormat);
}}

#endif	//KANI_TEX_FORMAT
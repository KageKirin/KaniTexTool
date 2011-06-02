//
//  kaniTexFormatTuple.h
//  KaniTexTool
//
//  Created by Christian Helmich on 03.06.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#pragma once
#ifndef KANI_TEX_FORMAT_TUPLE_H
#define KANI_TEX_FORMAT_TUPLE_H	1

#include "../core/kaniTypes.h"
#include "kaniTexFourCC.h"

#include <pvrtex/PVRTexLibGlobals.h>
#include <vector>

namespace kani { namespace texture {

	using pvrtexlib::PixelType;
	using std::vector;
	
	struct TexFormatTuple
	{
		const char*	text;
		PixelType	pvrtex;
		uint32		format;
		int			pngFormat;
		int			bitPerChannel;
		uint32		order;
		uint32		fourCC;
		
		TexFormatTuple(const char* t, PixelType p, uint32 f,
					   int pf, int bpc,
					   uint32 o, uint32 fcc);
	};
	
	typedef vector<TexFormatTuple>	TexFormatTupleMap;
	void createTexFormatTupleMap();

}}

#endif	//KANI_TEX_FORMAT_TUPLE_H
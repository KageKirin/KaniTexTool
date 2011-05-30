//
//  kaniTexPixelFormat.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 5/30/11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#include "kaniTexPixelFormat.h"
#include "kaniTexFormat.h"
#include <algorithm>

namespace kani { namespace texture {

	extern TexFormatTupleMap s_TexFormatTupleMap;

	template<typename predicate>
	const TexFormatTuple& findTuple(predicate P)
	{
		createTexFormatTupleMap();

		TexFormatTupleMap::iterator iter = find_if(s_TexFormatTupleMap.begin(), s_TexFormatTupleMap.end(), P);
		if(iter != s_TexFormatTupleMap.end())
			return *iter;
			
		return s_TexFormatTupleMap.at(0);
	}
	
	struct Predicate_FindByText
	{
		const char* text;
		
		Predicate_FindByText(const char* t):text(t){}		
		bool operator()(const TexFormatTuple& tuple)
		{
			return (strcmp(text, tuple.text) == 0);
		}
	};

	struct Predicate_FindByPVRPixelFormat
	{
		PixelType	pixelType;
		
		Predicate_FindByPVRPixelFormat(PixelType pt):pixelType(pt){}		
		bool operator()(const TexFormatTuple& tuple)
		{
			return (pixelType == tuple.pvrtex);
		}
	};
	
	struct Predicate_FindByPNGBits
	{
		int	pngFormat;
		int bitPerChannel;
		
		Predicate_FindByPNGBits(int pf, int bpc):
		pngFormat(pf), bitPerChannel(bpc)
		{}
		
		bool operator()(const TexFormatTuple& tuple)
		{
			return	(pngFormat == tuple.pngFormat)
				&&	(bitPerChannel == tuple.bitPerChannel);
		}

	};
	
	
	PixelType	getSupportedPixelType(const char* textFormat)
	{
		const TexFormatTuple& tuple = findTuple(Predicate_FindByText(textFormat));
		return tuple.pvrtex;
	}
	
			
	PixelType	getSupportedPixelType(int pngFormat, int bitPerChannel)
	{
		const TexFormatTuple& tuple = findTuple(Predicate_FindByPNGBits(pngFormat, bitPerChannel));
		return tuple.pvrtex;
	}

}}


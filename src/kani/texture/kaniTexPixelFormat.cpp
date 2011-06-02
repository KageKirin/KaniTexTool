//
//  kaniTexPixelFormat.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 5/30/11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#include "kaniTexPixelFormat.h"
#include "kaniTexFormat.h"
#include "kaniTexFormatTuple.h"
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
	
	const TexFormatTuple& getTexFormatTuple(const char* textFormat)
	{
		return findTuple(Predicate_FindByText(textFormat));
	}
	
	const TexFormatTuple& getTexFormatTuple(int pngFormat, int bitPerChannel)
	{
		return findTuple(Predicate_FindByPNGBits(pngFormat, bitPerChannel));
	}
	
	const TexFormatTuple& getTexFormatTuple(PixelType pixelType)
	{
		return findTuple(Predicate_FindByPVRPixelFormat(pixelType));
	}
	
	
	PixelType	getSupportedPixelType(const char* textFormat)
	{
		const TexFormatTuple& tuple = getTexFormatTuple(textFormat);
		return tuple.pvrtex;
	}
	
			
	PixelType	getSupportedPixelType(int pngFormat, int bitPerChannel)
	{
		const TexFormatTuple& tuple = getTexFormatTuple(pngFormat, bitPerChannel);
		return tuple.pvrtex;
	}

	
	PngFormatInfo	getPngFormatInfo(PixelType pixelType)
	{
		const TexFormatTuple& tuple = getTexFormatTuple(pixelType);
		PngFormatInfo rv = { tuple.pngFormat, tuple.bitPerChannel };
		return rv;
	}
}}


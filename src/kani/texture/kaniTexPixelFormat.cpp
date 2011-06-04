//
//  kaniTexPixelFormat.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 5/30/11.
//  Copyright 2011 KageKirin. All rights reserved.
//

//#include "stdafx.h"
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
	
	struct Predicate_FindByOrder
	{
		uint32	order;
		
		Predicate_FindByOrder(uint32 o):order(o){}		
		bool operator()(const TexFormatTuple& tuple)
		{
			return (order == tuple.order);
		}
	};
	
	struct Predicate_FindByFourCC
	{
		uint32	fourCC;
		
		Predicate_FindByFourCC(uint32 fcc):fourCC(fcc){}		
		bool operator()(const TexFormatTuple& tuple)
		{
			return (fourCC == tuple.fourCC);
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
	
	const TexFormatTuple& getTexFormatTuple(uint32 order)
	{
		return findTuple(Predicate_FindByOrder(order));
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
	
	PixelType	getSupportedPixelType(uint32 fourCC)
	{
		const TexFormatTuple& tuple =  findTuple(Predicate_FindByFourCC(fourCC));
		return tuple.pvrtex;
	}


	
	PngFormatInfo	getPngFormatInfo(PixelType pixelType)
	{
		const TexFormatTuple& tuple = getTexFormatTuple(pixelType);
		PngFormatInfo rv = { tuple.pngFormat, tuple.bitPerChannel };
		return rv;
	}
	
	PixelType	getPixelTypeForMask(uint32 rMask, uint32 gMask, uint32 bMask, uint32 aMask)
	{
		uint32 order = 0;
		if(rMask & 0xFFFF && gMask & 0xFFFF && bMask & 0xFFFF && aMask & 0xFFFF)
		{
			for(int i = 0; i < 4; ++i)
			{
				if(rMask & (0xF000 >> i))
					order &= 'R' << ((3 - i) * 2);

				if(gMask & (0xF000 >> i))
					order &= 'G' << ((3 - i) * 2);
				
				if(bMask & (0xF000 >> i))
					order &= 'B' << ((3 - i) * 2);
				
				if(aMask & (0xF000 >> i))
					order &= 'A' << ((3 - i) * 2);
			}
			
		}
		else if(rMask & 0xFFFFFFFF && gMask & 0xFFFFFFFF && bMask & 0xFFFFFFFF && aMask & 0xFFFFFFFF)
		{
			for(int i = 0; i < 4; ++i)
			{
				if(rMask & (0xFF000000 >> i))
					order &= 'R' << ((3 - i) * 4);
				
				if(gMask & (0xFF000000 >> i))
					order &= 'G' << ((3 - i) * 4);
				
				if(bMask & (0xFF000000 >> i))
					order &= 'B' << ((3 - i) * 4);
				
				if(aMask & (0xFF000000 >> i))
					order &= 'A' << ((3 - i) * 4);
			}
		}
		const TexFormatTuple& tuple = getTexFormatTuple(order);
		return tuple.pvrtex;
	}
	
	void	getMaskForPixelType(PixelType pt, uint32* rMask, uint32* gMask, uint32* bMask, uint32* aMask)
	{
		const TexFormatTuple& tuple = getTexFormatTuple(pt);
		*rMask = 0;
		*gMask = 0;
		*bMask = 0;
		*aMask = 0;
		
		for(int i = 0; i < 4; ++i)
		{
			if(tuple.order & ('R' << ((3 - i) * 4)))
				*rMask &= 0xFF000000 >> i;
			
			if(tuple.order & ('G' << ((3 - i) * 4)))
				*gMask &= 0xFF000000 >> i;
			
			if(tuple.order & ('B' << ((3 - i) * 4)))
				*bMask &= 0xFF000000 >> i;
			
			if(tuple.order & ('A' << ((3 - i) * 4)))
				*aMask &= 0xFF000000 >> i;
		}
	}
}}


//
//  kaniTexFormat.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 15.05.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "kaniTexFormat.h"

#include <vector>
#include <algorithm>
#include <cstring>
#include <boost/preprocessor/stringize.hpp>


namespace kani { namespace texture {

	using std::vector;

	struct TexFormatTuple
	{
		const char* text;
		PixelType	pvrtex;
		Format		format;
		
		TexFormatTuple(const char* t, PixelType p, Format f):
		text(t), pvrtex(p), format(f)
		{}
	};
	
	
#define M_CREATEMAP(r, data, elem)	\
	s_TexFormatTupleMap.push_back	\
	( TexFormatTuple(BOOST_PP_STRINGIZE(BITUPLE_1ST(elem)), (PixelType)BITUPLE_2ND(elem), BOOST_PP_CAT(data, BITUPLE_1ST(elem)))	);
	
	
	typedef vector<TexFormatTuple>	TexFormatTupleMap;
	static TexFormatTupleMap s_TexFormatTupleMap;
	
	void createTexFormatTupleMap()
	{
		using namespace pvrtexlib;
	
		static bool created = false;
		if(created)
			return;
			
		s_TexFormatTupleMap.clear();

		BOOST_PP_SEQ_FOR_EACH(M_CREATEMAP, Format_, FORMAT_SEQ) // Format_x		
		
		created = true;
	}
	
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
	
	
	PixelType	getSupportedPixelType(const char* textFormat)
	{
		const TexFormatTuple& tuple = findTuple(Predicate_FindByText(textFormat));
		return tuple.pvrtex;
	}
	

}}
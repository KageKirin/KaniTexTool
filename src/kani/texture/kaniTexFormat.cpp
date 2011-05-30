//
//  kaniTexFormat.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 15.05.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "kaniTexFormat.h"

#include <cstring>
#include <boost/preprocessor/stringize.hpp>


namespace kani { namespace texture {

	TexFormatTuple::TexFormatTuple(const char* t, PixelType p, Format f, int pf, int bpc):
	text(t), pvrtex(p), format(f),
	pngFormat(pf), bitPerChannel(bpc)
	{
	}
	
	TexFormatTupleMap s_TexFormatTupleMap;
	
	
#define M_CREATEMAP(r, data, elem)	\
	s_TexFormatTupleMap.push_back	\
	( TexFormatTuple(BOOST_PP_STRINGIZE(QUADTUPLE_1ST(elem)), \
		(PixelType)QUADTUPLE_2ND(elem), \
		BOOST_PP_CAT(data, QUADTUPLE_1ST(elem)),\
		QUADTUPLE_3RD(elem),\
		QUADTUPLE_4TH(elem)));
	
	
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

}}
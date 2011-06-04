//
//  kaniTexFormat.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 15.05.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

//#include "stdafx.h"
#include "kaniTexFormat.h"
#include "kaniTexFormatTuple.h"

#include <cstring>
#include <boost/preprocessor/stringize.hpp>


namespace kani { namespace texture {

	
	TexFormatTupleMap s_TexFormatTupleMap;
	
	
#define M_CREATEMAP(r, data, elem)	\
	s_TexFormatTupleMap.push_back	\
	( TexFormatTuple(BOOST_PP_STRINGIZE(SEXTUPLE_1ST(elem)), \
		(PixelType)SEXTUPLE_2ND(elem), \
		(uint32)BOOST_PP_CAT(data, SEXTUPLE_1ST(elem)),\
		SEXTUPLE_3RD(elem),	\
		SEXTUPLE_4TH(elem),	\
		(uint32)SEXTUPLE_5TH(elem),	\
		SEXTUPLE_6TH(elem)	\
	));
	
	
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
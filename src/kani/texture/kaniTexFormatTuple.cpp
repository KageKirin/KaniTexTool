//
//  kaniTexFormatTuple.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 03.06.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

//#include "stdafx.h"
#include "kaniTexFormatTuple.h"

namespace kani { namespace texture {


	TexFormatTuple::TexFormatTuple(const char* t, PixelType p, uint32 f,
								   int pf, int bpc,
								   uint32 o, uint32 fcc):
	text(t), pvrtex(p), format(f),
	pngFormat(pf), bitPerChannel(bpc),
	order(o), fourCC(fcc)
	{
	}
	
}}
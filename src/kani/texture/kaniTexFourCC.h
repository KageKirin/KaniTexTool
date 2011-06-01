//
//  kaniTexFourCC.h
//  KaniTexTool
//
//  Created by Christian Helmich on 6/1/11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#pragma once
#ifndef KANI_TEX_FOURCC_H
#define	KANI_TEX_FOURCC_H	1

#include "../core/kaniTypes.h"
#include "../core/kaniEndian.h"

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

namespace kani { namespace texture {
	
	using kani::uint32;
	using kani::endian::static_swap;

#define	MAKE_FOURCC(fourcc)							static_swap<uint32>::swap<uint32(fourcc)>::value
#define	MAKE_FOURCC_ENUM_VALUE(base, name, fourcc)	BOOST_PP_CAT(base, name) = MAKE_FOURCC(fourcc)

#define M_MAKE_FOURCC_ENUM(r, data, elem)	\
	MAKE_FOURCC_ENUM_VALUE(data,			\
		BOOST_PP_TUPLE_ELEM(2, 0, elem),	\
		BOOST_PP_TUPLE_ELEM(2, 1, elem)		\
	),										\


//dds standard fourcc's
#define DDS_FOURCC_SEQ	\
	((DXT1, 'DXT1'))	\
	((DXT2, 'DXT2'))	\
	((DXT3, 'DXT3'))	\
	((DXT4, 'DXT4'))	\
	((DXT5, 'DXT5'))	\

	
//non-standard fourcc's	
#define	PVR_FOURCC_SEQ	\
	((PVR2, 'PVR2'))	\
	((PVR4, 'PVR4'))	\
	((ETC1, 'ETC1'))	\

//extend here
#define	EXT_FOURCC_SEQ	\



#define FOURCC_SEQ	\
	DDS_FOURCC_SEQ	\
	PVR_FOURCC_SEQ	\
	EXT_FOURCC_SEQ	\

	
	enum FourCC
	{
		BOOST_PP_SEQ_FOR_EACH(M_MAKE_FOURCC_ENUM, FourCC_, FOURCC_SEQ) 
	};
	
}}

#endif	//KANI_TEX_FOURCC_H
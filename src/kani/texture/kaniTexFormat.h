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

#include "../core/kaniTypes.h"
#include <pvrtex/PVRTexLibGlobals.h>
#include <libpng/png.h>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <vector>

namespace kani { namespace texture {

	using pvrtexlib::PixelType;
	using std::vector;


	//TODO: expand seq element to be tuple(string, pvrtexlib PixelType, ...)
#define BITUPLE_SAME(x)		x, x
#define DEFAULT_FORMAT_SEQ	\
	((SameAsInput,	-1, -1, 0,	0))	\
	\
	((A4R4G4B4,		MGLPT_ARGB_4444,	PNG_COLOR_TYPE_RGBA, 4,	'ARGB'))	\
	((A1R5G5B5,		MGLPT_ARGB_1555,	-1, 0,	'ARGB'))					\
	((R5G6B5,		MGLPT_RGB_565,		-1, 0,	'RGB'))						\
	((R5G5B5,		MGLPT_RGB_555,		PNG_COLOR_TYPE_RGB, 5,	'ARGB'))	\
	((A8R8G8B8,		MGLPT_ARGB_8888,	PNG_COLOR_TYPE_RGBA, 8,	'ARGB'))	\
	((A8R3G3B3,		MGLPT_ARGB_8332,	-1, 0,	'ARGB'))					\
	((I8,			MGLPT_I_8,			PNG_COLOR_TYPE_GRAY, 8,	'I'))		\
	((A8,			OGL_A_8,			-1, 0,	'A'))						\
	((A8I8,			MGLPT_AI_88,		PNG_COLOR_TYPE_GA, 8,	'AI'))		\
	((BITMAP,		MGLPT_1_BPP,		-1, 0,	'I'))						\
	((VY1UY0,		MGLPT_VY1UY0,		-1, 0,	'VUY'))						\
	((Y1VY0U,		MGLPT_Y1VY0U,		-1, 0,	'YVU'))						\
	((R4G44BA4,		OGL_RGBA_4444,		PNG_COLOR_TYPE_RGBA, 4,	'RGBA'))	\
	((R5G5B5A1,		OGL_RGBA_5551,		-1, 0,	'RGBA'))					\
	((R8G8B8A8,		OGL_RGBA_8888,		PNG_COLOR_TYPE_RGBA, 8,	'RGBA'))	\
	((R8G8B8,		OGL_RGB_888,		PNG_COLOR_TYPE_RGB, 8,	'RGBA'))	\
	((B8G8R8A8,		OGL_BGRA_8888,		PNG_COLOR_TYPE_RGBA, 8,	'BGRA'))	\
	\
	((PVRTC2,		MGLPT_PVRTC2,		-1, 2,	'RGBA'))		\
	((PVRTC4,		MGLPT_PVRTC4,		-1, 4,	'RGBA'))		\
	((DXT1,			D3D_DXT1,			-1, 0,	'RGBA'))		\
	((DXT2,			D3D_DXT2,			-1, 0,	'RGBA'))		\
	((DXT3,			D3D_DXT3,			-1, 0,	'RGBA'))		\
	((DXT4,			D3D_DXT4,			-1, 0,	'RGBA'))		\
	((DXT5,			D3D_DXT5,			-1, 0,	'RGBA'))		\
	((ETC,			ETC_RGB_4BPP,		-1, 4,	'RGB'))			\
	\
	((R3G3B2,		D3D_RGB_332,		-1, 0,	'RGB'))			\
	((A10B10G10R10,	D3D_ABGR_2101010,	-1, 10,	'ABGR'))		\
	((A10R10G10B10,	D3D_ARGB_2101010,	-1, 10,	'ARGB'))		\
	((G16R16,		D3D_GR_1616,		-1, 16,	'GR'))			\
	((V16U16,		D3D_VU_1616,		-1, 16,	'VU'))			\
	((A16B16G16R16,	D3D_ABGR_16161616,	-1, 16,	'ABGR'))		\
	((R16F,			D3D_R16F,			-1, 16,	'R'))			\
	((GR16F,		D3D_GR_1616F,		-1, 16,	'GR'))			\
	((ABGR16F,		D3D_ABGR_16161616F,	-1, 16,	'ABGR'))		\
	((R32F,			D3D_R32F,			-1, 32,	'R'))			\
	((GR32F,		D3D_GR_3232F,		-1, 32,	'GR'))			\
	((ABGR32F,		D3D_ABGR_32323232F,	-1, 32,	'ABGR'))		\
	\
	((GL_R4G4B4A4,	OGL_RGBA_4444,		PNG_COLOR_TYPE_RGBA, 4,	'RGBA'))	\
	((GL_R5G5B5A1,	OGL_RGBA_5551,		-1, 0,	'RGBA'))					\
	((GL_R8G8B8A8,	OGL_RGBA_8888,		PNG_COLOR_TYPE_RGBA, 8,	'RGBA'))	\
	((GL_R5G6B5,	OGL_RGB_565,		-1, 0,	'RGB'))						\
	((GL_R5G5B5,	OGL_RGB_555,		PNG_COLOR_TYPE_RGBA, 5,	'RGB'))		\
	((GL_R8G8B8,	OGL_RGB_888,		PNG_COLOR_TYPE_RGB, 8,	'RGB'))		\
	((GL_I8,		OGL_I_8,			PNG_COLOR_TYPE_GRAY, 8,	'I'))		\
	((GL_A8,		OGL_A_8,			-1, 0,	'A'))						\
	((GL_A8I8,		OGL_AI_88,			PNG_COLOR_TYPE_GA, 8,	'AI'))		\
	((GL_PVRTC2,	OGL_PVRTC2,			-1, 2,	'RGBA'))					\
	((GL_PVRTC4,	OGL_PVRTC4,			-1, 4,	'RGBA'))					\
	((GL_B8G8R8A8,	OGL_BGRA_8888,		PNG_COLOR_TYPE_RGBA, 8,	'BGRA'))	\


#define	EXTENDED_FORMAT_SEQ	\
	((BITUPLE_SAME(MGLPT_ARGB_4444),	PNG_COLOR_TYPE_RGBA, 4,	'ARGB'))	\
	((BITUPLE_SAME(MGLPT_ARGB_1555),	-1, 0,	'ARGB'))					\
	((BITUPLE_SAME(MGLPT_RGB_565),		-1, 0,	'RGB'))						\
	((BITUPLE_SAME(MGLPT_RGB_555),		PNG_COLOR_TYPE_RGB, 5,	'RGB'))		\
	((BITUPLE_SAME(MGLPT_RGB_888),		PNG_COLOR_TYPE_RGB, 8,	'RGB'))		\
	((BITUPLE_SAME(MGLPT_ARGB_8888),	PNG_COLOR_TYPE_RGBA, 8,	'ARGB'))	\
	((BITUPLE_SAME(MGLPT_ARGB_8332),	-1, 0,	'ARGB'))					\
	((BITUPLE_SAME(MGLPT_I_8),			PNG_COLOR_TYPE_GRAY, 8,	'I'))		\
	((BITUPLE_SAME(MGLPT_AI_88),		PNG_COLOR_TYPE_GA, 8,	'AI'))		\
	((BITUPLE_SAME(MGLPT_1_BPP),		-1, 0,	'I'))						\
	((BITUPLE_SAME(MGLPT_VY1UY0),		-1, 0,	'VYU'))						\
	((BITUPLE_SAME(MGLPT_Y1VY0U),		-1, 0,	'YVU'))						\
	((BITUPLE_SAME(MGLPT_PVRTC2),		-1, 2,	'RGBA'))					\
	((BITUPLE_SAME(MGLPT_PVRTC4),		-1, 4,	'RGBA'))					\


#define	GL_FORMAT_SEQ	\
	((BITUPLE_SAME(OGL_RGBA_4444),		PNG_COLOR_TYPE_RGBA, 4,	'RGBA'))	\
	((BITUPLE_SAME(OGL_RGBA_5551),		-1, 0,	'RGBA'))					\
	((BITUPLE_SAME(OGL_RGBA_8888),		PNG_COLOR_TYPE_RGBA, 8,	'RGBA'))	\
	((BITUPLE_SAME(OGL_RGB_565),		-1, 0,	'RGB'))						\
	((BITUPLE_SAME(OGL_RGB_555),		PNG_COLOR_TYPE_RGB, 5,	'RGB'))		\
	((BITUPLE_SAME(OGL_RGB_888),		PNG_COLOR_TYPE_RGB, 8,	'RGB'))		\
	((BITUPLE_SAME(OGL_I_8),			PNG_COLOR_TYPE_GRAY, 8,	'I'))		\
	((BITUPLE_SAME(OGL_AI_88),			PNG_COLOR_TYPE_GA, 8,	'AI'))		\
	((BITUPLE_SAME(OGL_PVRTC2),			-1, 2,	'RGBA'))					\
	((BITUPLE_SAME(OGL_PVRTC4),			-1, 4,	'RGBA'))					\
	((BITUPLE_SAME(OGL_BGRA_8888),		PNG_COLOR_TYPE_RGBA, 8,	'BGRA'))	\
	((BITUPLE_SAME(OGL_A_8),			PNG_COLOR_TYPE_GRAY, 8,	'A'))		\


#define	D3D_FORMAT_SEQ	\
	((BITUPLE_SAME(D3D_DXT1),			-1, 0,	'RGBA'))		\
	((BITUPLE_SAME(D3D_DXT2),			-1, 0,	'RGBA'))		\
	((BITUPLE_SAME(D3D_DXT3),			-1, 0,	'RGBA'))		\
	((BITUPLE_SAME(D3D_DXT4),			-1, 0,	'RGBA'))		\
	((BITUPLE_SAME(D3D_DXT5),			-1, 0,	'RGBA'))		\
	\
	((BITUPLE_SAME(D3D_RGB_332),		-1, 0,	'RGB'))			\
	((BITUPLE_SAME(D3D_AL_44),			-1, 0,	'AI'))			\
	((BITUPLE_SAME(D3D_LVU_655),		-1, 0,	'IVU'))			\
	((BITUPLE_SAME(D3D_XLVU_8888),		-1, 8,	'XLVU'))		\
	((BITUPLE_SAME(D3D_QWVU_8888),		-1, 8,	'QWVU'))		\
	((BITUPLE_SAME(D3D_ABGR_2101010),	PNG_COLOR_TYPE_RGBA, 10,	'ABGR'))	\
	((BITUPLE_SAME(D3D_ARGB_2101010),	PNG_COLOR_TYPE_RGBA, 10,	'ARGB'))	\
	((BITUPLE_SAME(D3D_AWVU_2101010),	-1, 10,	'AWVU'))		\
	((BITUPLE_SAME(D3D_GR_1616),		-1, 16,	'GR'))			\
	((BITUPLE_SAME(D3D_VU_1616),		-1, 16,	'VU'))			\
	((BITUPLE_SAME(D3D_ABGR_16161616),	PNG_COLOR_TYPE_RGBA, 16,	'ABGR'))	\
	((BITUPLE_SAME(D3D_R16F),			-1, 16,	'R'))			\
	((BITUPLE_SAME(D3D_GR_1616F),		-1, 16,	'GR'))			\
	((BITUPLE_SAME(D3D_ABGR_16161616F),	-1, 16,	'ABGR'))		\
	((BITUPLE_SAME(D3D_R32F),			-1, 32,	'R'))			\
	((BITUPLE_SAME(D3D_GR_3232F),		-1, 32,	'GR'))			\
	((BITUPLE_SAME(D3D_ABGR_32323232F),	-1, 32,	'ABGR'))		\
	((BITUPLE_SAME(D3D_A8),				-1, 8,	'A'))			\
	((BITUPLE_SAME(D3D_V8U8),			-1, 8,	'VU'))			\
	((BITUPLE_SAME(D3D_L16),			-1, 16,	'I'))			\
	((BITUPLE_SAME(D3D_L8),				-1, 8,	'I'))			\
	((BITUPLE_SAME(D3D_AL_88),			-1, 8,	'AI'))			\
	((BITUPLE_SAME(D3D_UYVY),			-1, 8,	'UYVY'))		\
	((BITUPLE_SAME(D3D_YUY2),			-1, 8,	'YUY2'))		\


#define	DX10_FORMAT_SEQ	\
	((BITUPLE_SAME(DX10_R32G32B32A32_FLOAT),	-1, 32,	'RGBA'))		\
	((BITUPLE_SAME(DX10_R32G32B32A32_UINT),		-1,	32,	'RGBA'))		\
	((BITUPLE_SAME(DX10_R32G32B32A32_SINT),		-1, 32,	'RGBA'))		\
	((BITUPLE_SAME(DX10_R32G32B32_FLOAT),		-1, 32,	'RGB'))			\
	((BITUPLE_SAME(DX10_R32G32B32_UINT),		-1, 32,	'RGB'))			\
	((BITUPLE_SAME(DX10_R32G32B32_SINT),		-1, 32,	'RGB'))			\
	((BITUPLE_SAME(DX10_R16G16B16A16_FLOAT),	-1, 16,	'RGBA'))		\
	((BITUPLE_SAME(DX10_R16G16B16A16_UNORM),	-1, 16,	'RGBA'))		\
	((BITUPLE_SAME(DX10_R16G16B16A16_UINT),		-1, 16,	'RGBA'))		\
	((BITUPLE_SAME(DX10_R16G16B16A16_SNORM),	-1, 16,	'RGBA'))		\
	((BITUPLE_SAME(DX10_R16G16B16A16_SINT),		-1, 16,	'RGBA'))		\
	((BITUPLE_SAME(DX10_R32G32_FLOAT),			-1, 32,	'RG'))			\
	((BITUPLE_SAME(DX10_R32G32_UINT),			-1, 32,	'RG'))			\
	((BITUPLE_SAME(DX10_R32G32_SINT),			-1, 32,	'RG'))			\
	((BITUPLE_SAME(DX10_R10G10B10A2_UNORM),		-1, 10,	'RGBA'))		\
	((BITUPLE_SAME(DX10_R10G10B10A2_UINT),		-1, 10,	'RGBA'))		\
	((BITUPLE_SAME(DX10_R8G8B8A8_UNORM),		-1, 8,	'RGBA'))		\
	((BITUPLE_SAME(DX10_R8G8B8A8_UNORM_SRGB),	-1, 8,	'RGBA'))		\
	((BITUPLE_SAME(DX10_R8G8B8A8_UINT),			-1, 8,	'RGBA'))		\
	((BITUPLE_SAME(DX10_R8G8B8A8_SNORM),		-1, 8,	'RGBA'))		\
	((BITUPLE_SAME(DX10_R8G8B8A8_SINT),			-1, 8,	'RGBA'))		\
	((BITUPLE_SAME(DX10_R16G16_FLOAT),			-1, 16,	'RG'))			\
	((BITUPLE_SAME(DX10_R16G16_UNORM),			-1, 16,	'RG'))			\
	((BITUPLE_SAME(DX10_R16G16_UINT),			-1, 16,	'RG'))			\
	((BITUPLE_SAME(DX10_R16G16_SNORM),			-1, 16,	'RG'))			\
	((BITUPLE_SAME(DX10_R16G16_SINT),			-1, 16,	'RG'))			\
	((BITUPLE_SAME(DX10_R32_FLOAT),				-1, 32,	'R'))			\
	((BITUPLE_SAME(DX10_R32_UINT),				-1, 32,	'R'))			\
	((BITUPLE_SAME(DX10_R32_SINT),				-1, 32,	'R'))			\
	((BITUPLE_SAME(DX10_R8G8_UNORM),			-1, 8,	'RG'))			\
	((BITUPLE_SAME(DX10_R8G8_UINT),				-1, 8,	'RG'))			\
	((BITUPLE_SAME(DX10_R8G8_SNORM),			-1, 8,	'RG'))			\
	((BITUPLE_SAME(DX10_R8G8_SINT),				-1, 8,	'RG'))			\
	((BITUPLE_SAME(DX10_R16_FLOAT),				-1, 16,	'R'))			\
	((BITUPLE_SAME(DX10_R16_UNORM),				-1, 16,	'R'))			\
	((BITUPLE_SAME(DX10_R16_UINT),				-1, 16,	'R'))			\
	((BITUPLE_SAME(DX10_R16_SNORM),				-1, 16,	'R'))			\
	((BITUPLE_SAME(DX10_R16_SINT),				-1, 16,	'R'))			\
	((BITUPLE_SAME(DX10_R8_UNORM),				-1, 8,	'R'))			\
	((BITUPLE_SAME(DX10_R8_UINT),				-1, 8,	'R'))			\
	((BITUPLE_SAME(DX10_R8_SNORM),				-1, 8,	'R'))			\
	((BITUPLE_SAME(DX10_R8_SINT),				-1, 8,	'R'))			\
	((BITUPLE_SAME(DX10_A8_UNORM),				-1, 8,	'R'))			\
	((BITUPLE_SAME(DX10_R1_UNORM),				-1, 1,	'R'))			\
	((BITUPLE_SAME(DX10_BC1_UNORM),				-1, 0,	'BC1'))			\
	((BITUPLE_SAME(DX10_BC1_UNORM_SRGB),		-1, 0,	'BC1'))			\
	((BITUPLE_SAME(DX10_BC2_UNORM),				-1, 0,	'BC2'))			\
	((BITUPLE_SAME(DX10_BC2_UNORM_SRGB),		-1, 0,	'BC2'))			\
	((BITUPLE_SAME(DX10_BC3_UNORM),				-1, 0,	'BC3'))			\
	((BITUPLE_SAME(DX10_BC3_UNORM_SRGB),		-1, 0,	'BC3'))			\


#define	VG_FORMAT_SEQ	\
	((BITUPLE_SAME(ePT_VG_sRGBX_8888),			-1, 8,	'RGBX'))		\
	((BITUPLE_SAME(ePT_VG_sRGBA_8888),			-1, 8,	'RGBA'))		\
	((BITUPLE_SAME(ePT_VG_sRGBA_8888_PRE),		-1, 8,	'RGBA'))		\
	((BITUPLE_SAME(ePT_VG_sRGB_565),			-1, 8,	'RGB'))			\
	((BITUPLE_SAME(ePT_VG_sRGBA_5551),			-1, 5,	'RGBA'))		\
	((BITUPLE_SAME(ePT_VG_sRGBA_4444),			-1, 4,	'RGBA'))		\
	((BITUPLE_SAME(ePT_VG_sL_8),				-1, 8,	'I'))			\
	((BITUPLE_SAME(ePT_VG_lRGBX_8888),			-1, 8,	'RGBX'))		\
	((BITUPLE_SAME(ePT_VG_lRGBA_8888),			-1, 8,	'RGBA'))		\
	((BITUPLE_SAME(ePT_VG_lRGBA_8888_PRE),		-1, 8,	'RGBA'))		\
	((BITUPLE_SAME(ePT_VG_lL_8),				-1, 8,	'I'))			\
	((BITUPLE_SAME(ePT_VG_A_8),					-1, 8,	'A'))			\
	((BITUPLE_SAME(ePT_VG_BW_1),				-1, 1,	'I'))			\
	((BITUPLE_SAME(ePT_VG_sXRGB_8888),			-1, 8,	'XRGB'))		\
	((BITUPLE_SAME(ePT_VG_sARGB_8888),			-1, 8,	'ARGB'))		\
	((BITUPLE_SAME(ePT_VG_sARGB_8888_PRE),		-1, 8,	'ARGB'))		\
	((BITUPLE_SAME(ePT_VG_sARGB_1555),			-1, 5,	'ARGB'))		\
	((BITUPLE_SAME(ePT_VG_sARGB_4444),			-1, 4,	'ARGB'))		\
	((BITUPLE_SAME(ePT_VG_lXRGB_8888),			-1, 8,	'XRGB'))		\
	((BITUPLE_SAME(ePT_VG_lARGB_8888),			-1, 8,	'ARGB'))		\
	((BITUPLE_SAME(ePT_VG_lARGB_8888_PRE),		-1, 8,	'ARGB'))		\
	((BITUPLE_SAME(ePT_VG_sBGRX_8888),			-1, 8,	'BGRX'))		\
	((BITUPLE_SAME(ePT_VG_sBGRA_8888),			-1, 8,	'BGRA'))		\
	((BITUPLE_SAME(ePT_VG_sBGRA_8888_PRE),		-1, 8,	'BGRA'))		\
	((BITUPLE_SAME(ePT_VG_sBGR_565),			-1, 5,	'BGR'))			\
	((BITUPLE_SAME(ePT_VG_sBGRA_5551),			-1, 5,	'BGRA'))		\
	((BITUPLE_SAME(ePT_VG_sBGRA_4444),			-1, 4,	'BGRA'))		\
	((BITUPLE_SAME(ePT_VG_lBGRX_8888),			-1, 8,	'BGRX'))		\
	((BITUPLE_SAME(ePT_VG_lBGRA_8888),			-1, 8,	'BGRA'))		\
	((BITUPLE_SAME(ePT_VG_lBGRA_8888_PRE),		-1, 8,	'BGRA'))		\
	((BITUPLE_SAME(ePT_VG_sXBGR_8888),			-1, 8,	'XBGR'))		\
	((BITUPLE_SAME(ePT_VG_sABGR_8888),			-1, 8,	'ABGR'))		\
	((BITUPLE_SAME(ePT_VG_sABGR_8888_PRE),		-1, 8,	'ABGR'))		\
	((BITUPLE_SAME(ePT_VG_sABGR_1555),			-1, 5,	'ABGR'))		\
	((BITUPLE_SAME(ePT_VG_sABGR_4444),			-1, 4,	'ABGR'))		\
	((BITUPLE_SAME(ePT_VG_lXBGR_8888),			-1, 8,	'XBGR'))		\
	((BITUPLE_SAME(ePT_VG_lABGR_8888),			-1, 8,	'ABGR'))		\
	((BITUPLE_SAME(ePT_VG_lABGR_8888_PRE),		-1, 8,	'ABGR'))		\



#define	FORMAT_SEQ	\
	DEFAULT_FORMAT_SEQ	\
	EXTENDED_FORMAT_SEQ	\
	GL_FORMAT_SEQ		\
	D3D_FORMAT_SEQ		\
	DX10_FORMAT_SEQ		\
	VG_FORMAT_SEQ		\


#define QUINTUPLE_1ST(tuple)	BOOST_PP_TUPLE_ELEM(5,0, tuple)
#define QUINTUPLE_2ND(tuple)	BOOST_PP_TUPLE_ELEM(5,1, tuple)
#define QUINTUPLE_3RD(tuple)	BOOST_PP_TUPLE_ELEM(5,2, tuple)
#define QUINTUPLE_4TH(tuple)	BOOST_PP_TUPLE_ELEM(5,3, tuple)
#define QUINTUPLE_5TH(tuple)	BOOST_PP_TUPLE_ELEM(5,4, tuple)

#define M_CAT_1ST(r, data, elem) BOOST_PP_CAT(data, QUINTUPLE_1ST(elem)),
		
	enum Format
	{
		BOOST_PP_SEQ_FOR_EACH(M_CAT_1ST, Format_, FORMAT_SEQ) // Format_x
		Format_Count		
	};
#undef M_CAT	

	struct TexFormatTuple
	{
		const char*	text;
		PixelType	pvrtex;
		Format		format;
		int			pngFormat;
		int			bitPerChannel;
		uint32		order;
		
		TexFormatTuple(const char* t, PixelType p, Format f, int pf, int bpc, uint32 o);
	};
	
	typedef vector<TexFormatTuple>	TexFormatTupleMap;
	void createTexFormatTupleMap();
}}

#endif	//KANI_TEX_FORMAT
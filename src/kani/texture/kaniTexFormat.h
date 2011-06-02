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
#include "kaniTexFourCC.h"

#include <pvrtex/PVRTexLibGlobals.h>
#include <libpng/png.h>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <vector>

namespace kani { namespace texture {

	using pvrtexlib::PixelType;

	//TODO: expand seq element to be tuple(string, pvrtexlib PixelType, ...)
	//TODO: add squishType (int) where appliable, 0 else
#define BITUPLE_SAME(x)		x, x
#define DEFAULT_FORMAT_SEQ	\
	((SameAsInput,	-1, -1, 0,	0,	0))	\
	\
	((A4R4G4B4,		MGLPT_ARGB_4444,	PNG_COLOR_TYPE_RGBA, 4,	'ARGB',	0))	\
	((A1R5G5B5,		MGLPT_ARGB_1555,	-1, 0,	'ARGB',	0))					\
	((R5G6B5,		MGLPT_RGB_565,		-1, 0,	'RGB',	0))					\
	((R5G5B5,		MGLPT_RGB_555,		PNG_COLOR_TYPE_RGB, 5,	'ARGB',	0))	\
	((A8R8G8B8,		MGLPT_ARGB_8888,	PNG_COLOR_TYPE_RGBA, 8,	'ARGB',	0))	\
	((A8R3G3B3,		MGLPT_ARGB_8332,	-1, 0,	'ARGB',	0))					\
	((I8,			MGLPT_I_8,			PNG_COLOR_TYPE_GRAY, 8,	'I',	0))	\
	((A8,			OGL_A_8,			-1, 0,	'A',	0))					\
	((A8I8,			MGLPT_AI_88,		PNG_COLOR_TYPE_GA, 8,	'AI',	0))	\
	((BITMAP,		MGLPT_1_BPP,		-1, 0,	'I',	0))					\
	((VY1UY0,		MGLPT_VY1UY0,		-1, 0,	'VUY',	0))					\
	((Y1VY0U,		MGLPT_Y1VY0U,		-1, 0,	'YVU',	0))					\
	((R4G44BA4,		OGL_RGBA_4444,		PNG_COLOR_TYPE_RGBA, 4,	'RGBA',	0))	\
	((R5G5B5A1,		OGL_RGBA_5551,		-1, 0,	'RGBA',	0))					\
	((R8G8B8A8,		OGL_RGBA_8888,		PNG_COLOR_TYPE_RGBA, 8,	'RGBA',	0))	\
	((R8G8B8,		OGL_RGB_888,		PNG_COLOR_TYPE_RGB, 8,	'RGBA',	0))	\
	((B8G8R8A8,		OGL_BGRA_8888,		PNG_COLOR_TYPE_RGBA, 8,	'BGRA',	0))	\
	\
	((PVRTC2,		MGLPT_PVRTC2,		-1, 2,	'RGBA',	FourCC_PVR2))		\
	((PVRTC4,		MGLPT_PVRTC4,		-1, 4,	'RGBA',	FourCC_PVR4))		\
	((DXT1,			D3D_DXT1,			-1, 0,	'RGBA',	FourCC_DXT1))		\
	((DXT2,			D3D_DXT2,			-1, 0,	'RGBA',	FourCC_DXT2))		\
	((DXT3,			D3D_DXT3,			-1, 0,	'RGBA',	FourCC_DXT3))		\
	((DXT4,			D3D_DXT4,			-1, 0,	'RGBA',	FourCC_DXT4))		\
	((DXT5,			D3D_DXT5,			-1, 0,	'RGBA',	FourCC_DXT5))		\
	((ETC,			ETC_RGB_4BPP,		-1, 4,	'RGB',	FourCC_ETC1))		\
	\
	((R3G3B2,		D3D_RGB_332,		-1, 0,	'RGB',	0))			\
	((A10B10G10R10,	D3D_ABGR_2101010,	-1, 10,	'ABGR',	0))			\
	((A10R10G10B10,	D3D_ARGB_2101010,	-1, 10,	'ARGB',	0))			\
	((G16R16,		D3D_GR_1616,		-1, 16,	'GR',	0))			\
	((V16U16,		D3D_VU_1616,		-1, 16,	'VU',	0))			\
	((A16B16G16R16,	D3D_ABGR_16161616,	-1, 16,	'ABGR',	0))			\
	((R16F,			D3D_R16F,			-1, 16,	'R',	0))			\
	((GR16F,		D3D_GR_1616F,		-1, 16,	'GR',	0))			\
	((ABGR16F,		D3D_ABGR_16161616F,	-1, 16,	'ABGR',	0))			\
	((R32F,			D3D_R32F,			-1, 32,	'R',	0))			\
	((GR32F,		D3D_GR_3232F,		-1, 32,	'GR',	0))			\
	((ABGR32F,		D3D_ABGR_32323232F,	-1, 32,	'ABGR',	0))			\
	\
	((GL_R4G4B4A4,	OGL_RGBA_4444,		PNG_COLOR_TYPE_RGBA, 4,	'RGBA',	0))	\
	((GL_R5G5B5A1,	OGL_RGBA_5551,		-1, 0,					'RGBA',	0))	\
	((GL_R8G8B8A8,	OGL_RGBA_8888,		PNG_COLOR_TYPE_RGBA, 8,	'RGBA',	0))	\
	((GL_R5G6B5,	OGL_RGB_565,		-1, 0,					'RGB',	0))	\
	((GL_R5G5B5,	OGL_RGB_555,		PNG_COLOR_TYPE_RGBA, 5,	'RGB',	0))	\
	((GL_R8G8B8,	OGL_RGB_888,		PNG_COLOR_TYPE_RGB, 8,	'RGB',	0))	\
	((GL_I8,		OGL_I_8,			PNG_COLOR_TYPE_GRAY, 8,	'I',	0))	\
	((GL_A8,		OGL_A_8,			-1, 0,					'A',	0))	\
	((GL_A8I8,		OGL_AI_88,			PNG_COLOR_TYPE_GA, 8,	'AI',	0))	\
	((GL_PVRTC2,	OGL_PVRTC2,			-1, 2,					'RGBA',	0))	\
	((GL_PVRTC4,	OGL_PVRTC4,			-1, 4,					'RGBA',	0))	\
	((GL_B8G8R8A8,	OGL_BGRA_8888,		PNG_COLOR_TYPE_RGBA, 8,	'BGRA',	0))	\


#define	EXTENDED_FORMAT_SEQ	\
	((BITUPLE_SAME(MGLPT_ARGB_4444),	PNG_COLOR_TYPE_RGBA, 4,	'ARGB',	0))	\
	((BITUPLE_SAME(MGLPT_ARGB_1555),	-1, 0,					'ARGB',	0))	\
	((BITUPLE_SAME(MGLPT_RGB_565),		-1, 0,					'RGB',	0))	\
	((BITUPLE_SAME(MGLPT_RGB_555),		PNG_COLOR_TYPE_RGB, 5,	'RGB',	0))	\
	((BITUPLE_SAME(MGLPT_RGB_888),		PNG_COLOR_TYPE_RGB, 8,	'RGB',	0))	\
	((BITUPLE_SAME(MGLPT_ARGB_8888),	PNG_COLOR_TYPE_RGBA, 8,	'ARGB',	0))	\
	((BITUPLE_SAME(MGLPT_ARGB_8332),	-1, 0,					'ARGB',	0))	\
	((BITUPLE_SAME(MGLPT_I_8),			PNG_COLOR_TYPE_GRAY, 8,	'I',	0))	\
	((BITUPLE_SAME(MGLPT_AI_88),		PNG_COLOR_TYPE_GA, 8,	'AI',	0))	\
	((BITUPLE_SAME(MGLPT_1_BPP),		-1, 0,					'I',	0))	\
	((BITUPLE_SAME(MGLPT_VY1UY0),		-1, 0,					'VYU',	0))	\
	((BITUPLE_SAME(MGLPT_Y1VY0U),		-1, 0,					'YVU',	0))	\
	((BITUPLE_SAME(MGLPT_PVRTC2),		-1, 2,					'RGBA',	FourCC_PVR2))	\
	((BITUPLE_SAME(MGLPT_PVRTC4),		-1, 4,					'RGBA',	FourCC_PVR4))	\


#define	GL_FORMAT_SEQ	\
	((BITUPLE_SAME(OGL_RGBA_4444),		PNG_COLOR_TYPE_RGBA, 4,	'RGBA',	0))	\
	((BITUPLE_SAME(OGL_RGBA_5551),		-1, 0,					'RGBA',	0))	\
	((BITUPLE_SAME(OGL_RGBA_8888),		PNG_COLOR_TYPE_RGBA, 8,	'RGBA',	0))	\
	((BITUPLE_SAME(OGL_RGB_565),		-1, 0,					'RGB',	0))	\
	((BITUPLE_SAME(OGL_RGB_555),		PNG_COLOR_TYPE_RGB, 5,	'RGB',	0))	\
	((BITUPLE_SAME(OGL_RGB_888),		PNG_COLOR_TYPE_RGB, 8,	'RGB',	0))	\
	((BITUPLE_SAME(OGL_I_8),			PNG_COLOR_TYPE_GRAY, 8,	'I',	0))	\
	((BITUPLE_SAME(OGL_AI_88),			PNG_COLOR_TYPE_GA, 8,	'AI',	0))	\
	((BITUPLE_SAME(OGL_PVRTC2),			-1, 2,					'RGBA',	FourCC_PVR2))	\
	((BITUPLE_SAME(OGL_PVRTC4),			-1, 4,					'RGBA',	FourCC_PVR4))	\
	((BITUPLE_SAME(OGL_BGRA_8888),		PNG_COLOR_TYPE_RGBA, 8,	'BGRA',	0))	\
	((BITUPLE_SAME(OGL_A_8),			PNG_COLOR_TYPE_GRAY, 8,	'A',	0))	\


#define	D3D_FORMAT_SEQ	\
	((BITUPLE_SAME(D3D_DXT1),			-1, 0,	'RGBA',	FourCC_DXT1))		\
	((BITUPLE_SAME(D3D_DXT2),			-1, 0,	'RGBA',	FourCC_DXT2))		\
	((BITUPLE_SAME(D3D_DXT3),			-1, 0,	'RGBA',	FourCC_DXT3))		\
	((BITUPLE_SAME(D3D_DXT4),			-1, 0,	'RGBA',	FourCC_DXT4))		\
	((BITUPLE_SAME(D3D_DXT5),			-1, 0,	'RGBA',	FourCC_DXT5))		\
	\
	((BITUPLE_SAME(D3D_RGB_332),		-1, 0,	'RGB',	0))			\
	((BITUPLE_SAME(D3D_AL_44),			-1, 0,	'AI',	0))			\
	((BITUPLE_SAME(D3D_LVU_655),		-1, 0,	'IVU',	0))			\
	((BITUPLE_SAME(D3D_XLVU_8888),		-1, 8,	'XLVU',	0))			\
	((BITUPLE_SAME(D3D_QWVU_8888),		-1, 8,	'QWVU',	0))			\
	((BITUPLE_SAME(D3D_ABGR_2101010),	PNG_COLOR_TYPE_RGBA, 10,	'ABGR',	0))	\
	((BITUPLE_SAME(D3D_ARGB_2101010),	PNG_COLOR_TYPE_RGBA, 10,	'ARGB',	0))	\
	((BITUPLE_SAME(D3D_AWVU_2101010),	-1, 10,	'AWVU',	0))			\
	((BITUPLE_SAME(D3D_GR_1616),		-1, 16,	'GR',	0))			\
	((BITUPLE_SAME(D3D_VU_1616),		-1, 16,	'VU',	0))			\
	((BITUPLE_SAME(D3D_ABGR_16161616),	PNG_COLOR_TYPE_RGBA, 16,	'ABGR',	0))	\
	((BITUPLE_SAME(D3D_R16F),			-1, 16,	'R',	0))			\
	((BITUPLE_SAME(D3D_GR_1616F),		-1, 16,	'GR',	0))			\
	((BITUPLE_SAME(D3D_ABGR_16161616F),	-1, 16,	'ABGR',	0))			\
	((BITUPLE_SAME(D3D_R32F),			-1, 32,	'R',	0))			\
	((BITUPLE_SAME(D3D_GR_3232F),		-1, 32,	'GR',	0))			\
	((BITUPLE_SAME(D3D_ABGR_32323232F),	-1, 32,	'ABGR',	0))			\
	((BITUPLE_SAME(D3D_A8),				-1, 8,	'A',	0))			\
	((BITUPLE_SAME(D3D_V8U8),			-1, 8,	'VU',	0))			\
	((BITUPLE_SAME(D3D_L16),			-1, 16,	'I',	0))			\
	((BITUPLE_SAME(D3D_L8),				-1, 8,	'I',	0))			\
	((BITUPLE_SAME(D3D_AL_88),			-1, 8,	'AI',	0))			\
	((BITUPLE_SAME(D3D_UYVY),			-1, 8,	'UYVY',	0))			\
	((BITUPLE_SAME(D3D_YUY2),			-1, 8,	'YUY2',	0))			\


#define	DX10_FORMAT_SEQ	\
	((BITUPLE_SAME(DX10_R32G32B32A32_FLOAT),	-1, 32,	'RGBA',	0))		\
	((BITUPLE_SAME(DX10_R32G32B32A32_UINT),		-1,	32,	'RGBA',	0))		\
	((BITUPLE_SAME(DX10_R32G32B32A32_SINT),		-1, 32,	'RGBA',	0))		\
	((BITUPLE_SAME(DX10_R32G32B32_FLOAT),		-1, 32,	'RGB',	0))		\
	((BITUPLE_SAME(DX10_R32G32B32_UINT),		-1, 32,	'RGB',	0))		\
	((BITUPLE_SAME(DX10_R32G32B32_SINT),		-1, 32,	'RGB',	0))		\
	((BITUPLE_SAME(DX10_R16G16B16A16_FLOAT),	-1, 16,	'RGBA',	0))		\
	((BITUPLE_SAME(DX10_R16G16B16A16_UNORM),	-1, 16,	'RGBA',	0))		\
	((BITUPLE_SAME(DX10_R16G16B16A16_UINT),		-1, 16,	'RGBA',	0))		\
	((BITUPLE_SAME(DX10_R16G16B16A16_SNORM),	-1, 16,	'RGBA',	0))		\
	((BITUPLE_SAME(DX10_R16G16B16A16_SINT),		-1, 16,	'RGBA',	0))		\
	((BITUPLE_SAME(DX10_R32G32_FLOAT),			-1, 32,	'RG',	0))		\
	((BITUPLE_SAME(DX10_R32G32_UINT),			-1, 32,	'RG',	0))		\
	((BITUPLE_SAME(DX10_R32G32_SINT),			-1, 32,	'RG',	0))		\
	((BITUPLE_SAME(DX10_R10G10B10A2_UNORM),		-1, 10,	'RGBA',	0))		\
	((BITUPLE_SAME(DX10_R10G10B10A2_UINT),		-1, 10,	'RGBA',	0))		\
	((BITUPLE_SAME(DX10_R8G8B8A8_UNORM),		-1, 8,	'RGBA',	0))		\
	((BITUPLE_SAME(DX10_R8G8B8A8_UNORM_SRGB),	-1, 8,	'RGBA',	0))		\
	((BITUPLE_SAME(DX10_R8G8B8A8_UINT),			-1, 8,	'RGBA',	0))		\
	((BITUPLE_SAME(DX10_R8G8B8A8_SNORM),		-1, 8,	'RGBA',	0))		\
	((BITUPLE_SAME(DX10_R8G8B8A8_SINT),			-1, 8,	'RGBA',	0))		\
	((BITUPLE_SAME(DX10_R16G16_FLOAT),			-1, 16,	'RG',	0))		\
	((BITUPLE_SAME(DX10_R16G16_UNORM),			-1, 16,	'RG',	0))		\
	((BITUPLE_SAME(DX10_R16G16_UINT),			-1, 16,	'RG',	0))		\
	((BITUPLE_SAME(DX10_R16G16_SNORM),			-1, 16,	'RG',	0))		\
	((BITUPLE_SAME(DX10_R16G16_SINT),			-1, 16,	'RG',	0))		\
	((BITUPLE_SAME(DX10_R32_FLOAT),				-1, 32,	'R',	0))		\
	((BITUPLE_SAME(DX10_R32_UINT),				-1, 32,	'R',	0))		\
	((BITUPLE_SAME(DX10_R32_SINT),				-1, 32,	'R',	0))		\
	((BITUPLE_SAME(DX10_R8G8_UNORM),			-1, 8,	'RG',	0))		\
	((BITUPLE_SAME(DX10_R8G8_UINT),				-1, 8,	'RG',	0))		\
	((BITUPLE_SAME(DX10_R8G8_SNORM),			-1, 8,	'RG',	0))		\
	((BITUPLE_SAME(DX10_R8G8_SINT),				-1, 8,	'RG',	0))		\
	((BITUPLE_SAME(DX10_R16_FLOAT),				-1, 16,	'R',	0))		\
	((BITUPLE_SAME(DX10_R16_UNORM),				-1, 16,	'R',	0))		\
	((BITUPLE_SAME(DX10_R16_UINT),				-1, 16,	'R',	0))		\
	((BITUPLE_SAME(DX10_R16_SNORM),				-1, 16,	'R',	0))		\
	((BITUPLE_SAME(DX10_R16_SINT),				-1, 16,	'R',	0))		\
	((BITUPLE_SAME(DX10_R8_UNORM),				-1, 8,	'R',	0))		\
	((BITUPLE_SAME(DX10_R8_UINT),				-1, 8,	'R',	0))		\
	((BITUPLE_SAME(DX10_R8_SNORM),				-1, 8,	'R',	0))		\
	((BITUPLE_SAME(DX10_R8_SINT),				-1, 8,	'R',	0))		\
	((BITUPLE_SAME(DX10_A8_UNORM),				-1, 8,	'R',	0))		\
	((BITUPLE_SAME(DX10_R1_UNORM),				-1, 1,	'R',	0))		\
	((BITUPLE_SAME(DX10_BC1_UNORM),				-1, 0,	'BC1',	0))		\
	((BITUPLE_SAME(DX10_BC1_UNORM_SRGB),		-1, 0,	'BC1',	0))		\
	((BITUPLE_SAME(DX10_BC2_UNORM),				-1, 0,	'BC2',	0))		\
	((BITUPLE_SAME(DX10_BC2_UNORM_SRGB),		-1, 0,	'BC2',	0))		\
	((BITUPLE_SAME(DX10_BC3_UNORM),				-1, 0,	'BC3',	0))		\
	((BITUPLE_SAME(DX10_BC3_UNORM_SRGB),		-1, 0,	'BC3',	0))		\


#define	VG_FORMAT_SEQ	\
	((BITUPLE_SAME(ePT_VG_sRGBX_8888),			-1, 8,	'RGBX',	0))		\
	((BITUPLE_SAME(ePT_VG_sRGBA_8888),			-1, 8,	'RGBA',	0))		\
	((BITUPLE_SAME(ePT_VG_sRGBA_8888_PRE),		-1, 8,	'RGBA',	0))		\
	((BITUPLE_SAME(ePT_VG_sRGB_565),			-1, 8,	'RGB',	0))		\
	((BITUPLE_SAME(ePT_VG_sRGBA_5551),			-1, 5,	'RGBA',	0))		\
	((BITUPLE_SAME(ePT_VG_sRGBA_4444),			-1, 4,	'RGBA',	0))		\
	((BITUPLE_SAME(ePT_VG_sL_8),				-1, 8,	'I',	0))		\
	((BITUPLE_SAME(ePT_VG_lRGBX_8888),			-1, 8,	'RGBX',	0))		\
	((BITUPLE_SAME(ePT_VG_lRGBA_8888),			-1, 8,	'RGBA',	0))		\
	((BITUPLE_SAME(ePT_VG_lRGBA_8888_PRE),		-1, 8,	'RGBA',	0))		\
	((BITUPLE_SAME(ePT_VG_lL_8),				-1, 8,	'I',	0))		\
	((BITUPLE_SAME(ePT_VG_A_8),					-1, 8,	'A',	0))		\
	((BITUPLE_SAME(ePT_VG_BW_1),				-1, 1,	'I',	0))		\
	((BITUPLE_SAME(ePT_VG_sXRGB_8888),			-1, 8,	'XRGB',	0))		\
	((BITUPLE_SAME(ePT_VG_sARGB_8888),			-1, 8,	'ARGB',	0))		\
	((BITUPLE_SAME(ePT_VG_sARGB_8888_PRE),		-1, 8,	'ARGB',	0))		\
	((BITUPLE_SAME(ePT_VG_sARGB_1555),			-1, 5,	'ARGB',	0))		\
	((BITUPLE_SAME(ePT_VG_sARGB_4444),			-1, 4,	'ARGB',	0))		\
	((BITUPLE_SAME(ePT_VG_lXRGB_8888),			-1, 8,	'XRGB',	0))		\
	((BITUPLE_SAME(ePT_VG_lARGB_8888),			-1, 8,	'ARGB',	0))		\
	((BITUPLE_SAME(ePT_VG_lARGB_8888_PRE),		-1, 8,	'ARGB',	0))		\
	((BITUPLE_SAME(ePT_VG_sBGRX_8888),			-1, 8,	'BGRX',	0))		\
	((BITUPLE_SAME(ePT_VG_sBGRA_8888),			-1, 8,	'BGRA',	0))		\
	((BITUPLE_SAME(ePT_VG_sBGRA_8888_PRE),		-1, 8,	'BGRA',	0))		\
	((BITUPLE_SAME(ePT_VG_sBGR_565),			-1, 5,	'BGR',	0))		\
	((BITUPLE_SAME(ePT_VG_sBGRA_5551),			-1, 5,	'BGRA',	0))		\
	((BITUPLE_SAME(ePT_VG_sBGRA_4444),			-1, 4,	'BGRA',	0))		\
	((BITUPLE_SAME(ePT_VG_lBGRX_8888),			-1, 8,	'BGRX',	0))		\
	((BITUPLE_SAME(ePT_VG_lBGRA_8888),			-1, 8,	'BGRA',	0))		\
	((BITUPLE_SAME(ePT_VG_lBGRA_8888_PRE),		-1, 8,	'BGRA',	0))		\
	((BITUPLE_SAME(ePT_VG_sXBGR_8888),			-1, 8,	'XBGR',	0))		\
	((BITUPLE_SAME(ePT_VG_sABGR_8888),			-1, 8,	'ABGR',	0))		\
	((BITUPLE_SAME(ePT_VG_sABGR_8888_PRE),		-1, 8,	'ABGR',	0))		\
	((BITUPLE_SAME(ePT_VG_sABGR_1555),			-1, 5,	'ABGR',	0))		\
	((BITUPLE_SAME(ePT_VG_sABGR_4444),			-1, 4,	'ABGR',	0))		\
	((BITUPLE_SAME(ePT_VG_lXBGR_8888),			-1, 8,	'XBGR',	0))		\
	((BITUPLE_SAME(ePT_VG_lABGR_8888),			-1, 8,	'ABGR',	0))		\
	((BITUPLE_SAME(ePT_VG_lABGR_8888_PRE),		-1, 8,	'ABGR',	0))		\



#define	FORMAT_SEQ	\
	DEFAULT_FORMAT_SEQ	\
	EXTENDED_FORMAT_SEQ	\
	GL_FORMAT_SEQ		\
	D3D_FORMAT_SEQ		\
	DX10_FORMAT_SEQ		\
	VG_FORMAT_SEQ		\


#define SEXTUPLE_1ST(tuple)	BOOST_PP_TUPLE_ELEM(6,0, tuple)
#define SEXTUPLE_2ND(tuple)	BOOST_PP_TUPLE_ELEM(6,1, tuple)
#define SEXTUPLE_3RD(tuple)	BOOST_PP_TUPLE_ELEM(6,2, tuple)
#define SEXTUPLE_4TH(tuple)	BOOST_PP_TUPLE_ELEM(6,3, tuple)
#define SEXTUPLE_5TH(tuple)	BOOST_PP_TUPLE_ELEM(6,4, tuple)
#define SEXTUPLE_6TH(tuple)	BOOST_PP_TUPLE_ELEM(6,5, tuple)

#define M_CAT_1ST(r, data, elem) BOOST_PP_CAT(data, SEXTUPLE_1ST(elem)),
		
	enum Format
	{
		BOOST_PP_SEQ_FOR_EACH(M_CAT_1ST, Format_, FORMAT_SEQ) // Format_x
		Format_Count		
	};
#undef M_CAT	

}}

#endif	//KANI_TEX_FORMAT
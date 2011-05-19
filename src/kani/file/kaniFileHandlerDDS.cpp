//
//  kaniFileHandlerDDS.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 10.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#pragma once
#ifndef KANI_DDS_H
#define KANI_DDS_H 1

#include "../core/kaniTypes.h"

//--------------------------------------------
//DDS file defines

#define DDSD_CAPS             0x00000001l
#define DDSD_HEIGHT           0x00000002l
#define DDSD_WIDTH            0x00000004l
#define DDSD_PIXELFORMAT      0x00001000l
#define DDS_ALPHAPIXELS       0x00000001l
#define DDS_FOURCC            0x00000004l
#define DDS_PITCH             0x00000008l
#define DDS_COMPLEX           0x00000008l
#define DDS_RGB               0x00000040l
#define DDS_TEXTURE           0x00001000l
#define DDS_MIPMAPCOUNT       0x00020000l
#define DDS_LINEARSIZE        0x00080000l
#define DDS_VOLUME            0x00200000l
#define DDS_MIPMAP            0x00400000l
#define DDS_DEPTH             0x00800000l

#define DDS_CUBEMAP           0x00000200L
#define DDS_CUBEMAP_POSITIVEX 0x00000400L
#define DDS_CUBEMAP_NEGATIVEX 0x00000800L
#define DDS_CUBEMAP_POSITIVEY 0x00001000L
#define DDS_CUBEMAP_NEGATIVEY 0x00002000L
#define DDS_CUBEMAP_POSITIVEZ 0x00004000L
#define DDS_CUBEMAP_NEGATIVEZ 0x00008000L

#define FOURCC_DXT1 0x31545844 //(MAKEFOURCC('D','X','T','1'))
#define FOURCC_DXT3 0x33545844 //(MAKEFOURCC('D','X','T','3'))
#define FOURCC_DXT5 0x35545844 //(MAKEFOURCC('D','X','T','5'))

#define DDS_MAGIC_FLIPPED     0x0F7166ED

//--------------------------------------------
// DDS file format structures.

namespace kani { namespace dds {

	struct DDS_Pixelformat
	{
		uint32	dwSize;
		uint32	dwFlags;
		uint32	dwFourCC;
		uint32	dwRGBBitCount;
		uint32	dwRBitMask;
		uint32	dwGBitMask;
		uint32	dwBBitMask;
		uint32	dwABitMask;
	};
	
	struct DDS_Header 
	{
		uint32	dwSize;
		uint32	dwFlags;
		uint32	dwHeight;
		uint32	dwWidth;
		uint32	dwPitchOrLinearSize;
		uint32	dwDepth;
		uint32	dwMipMapCount;
		uint32	dwReserved1[11];
		DDS_Pixelformat	ddspf;
		uint32	dwCaps1;
		uint32	dwCaps2;
		uint32	dwReserved2[3];
	};

}}


#endif	//KANI_DDS_H

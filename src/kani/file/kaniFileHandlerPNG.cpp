//
//  kaniFileHandlerPNG.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 10.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

//#include "stdafx.h"
#include "kaniFileHandlerPNG.h"
#include "../texture/kaniTexPixelFormat.h"
#include <libpng/png.h>

#include <pvrtex/CPVRTexture.h>
#include <pvrtex/CPVRTextureHeader.h>
#include <pvrtex/CPVRTextureData.h>

#include <iostream>
#include <vector>
#include <cstdio>
#include <cassert>


#define PNGSIGSIZE 8


namespace kani { namespace file {
	
	//ref: @see http://www.libpng.org/pub/png/libpng-manual.txt
	//ref: @see http://www.piko3d.com/tutorials/libpng-tutorial-loading-png-files-from-streams
	//ref: @see http://zarb.org/~gc/html/libpng.html
	
	using std::ifstream;
	using std::ios_base;
	using std::cout;
	using std::cerr;
	using std::endl;
	using std::vector;
	
	//------------------------------------------------------------
	
	template<>
	int FileHandlerImpl<FileType_PNG>::internal_read(const string& filename, CPVRTextureHeader& pvrHeader, CPVRTextureData& pvrData) const
	{	
		FILE*	file = fopen(filename.c_str(), "rb");
		assert(file);
		if(!file)
			return -2;
				
		png_byte header[PNGSIGSIZE];		
		fread(header, sizeof(header), 1, file);
		int isPng = !png_sig_cmp(header, 0, sizeof(header));
		assert(isPng);
		if(!isPng)
			return -3;
		

		png_struct*	pPngStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING,
														NULL,	//user_error_ptr,
														NULL,	//user_error_fn,
														NULL	//user_warning_fn
														);
		assert(pPngStruct);
		if(!pPngStruct)
			return -4;

		png_info* pPngInfo = png_create_info_struct(pPngStruct);
		assert(pPngInfo);
		if(!pPngInfo)
		{
			png_destroy_read_struct(&pPngStruct, (png_info**)NULL, (png_info**)NULL);
			return -5;
		}

		png_info* pPngInfo_End = png_create_info_struct(pPngStruct);
		if(!pPngInfo_End)
		{
			png_destroy_read_struct(&pPngStruct, &pPngInfo, (png_infopp)NULL);
			return -5;
		}		
		
		//error handling
		if(setjmp(png_jmpbuf(pPngStruct)))
		{
			png_destroy_read_struct(&pPngStruct, &pPngInfo, &pPngInfo_End);
			fclose(file);
			return -6;
		}
		
		//proper file reading starts here
		png_init_io(pPngStruct, file);
		
		//increment internal read pointer by sizeof(header) since we already read it
	    png_set_sig_bytes(pPngStruct, sizeof(header));
		
		
		//optional: set special chunk handling
		//png_set_read_user_chunk_fn(pPngStruct, user_chunk_ptr, read_chunk_callback);
		
		//optional: set progress handling
		//png_set_read_status_fn(pPngStruct, read_row_callback);
	
		//read info	
		png_read_info(pPngStruct, pPngInfo);
		

		png_uint_32 width		= png_get_image_width(pPngStruct, pPngInfo);
		png_uint_32 height		= png_get_image_height(pPngStruct, pPngInfo);
		png_uint_32 bitDepth	= png_get_bit_depth(pPngStruct, pPngInfo);
		png_uint_32 channels	= png_get_channels(pPngStruct, pPngInfo);
		png_uint_32 colorType	= png_get_color_type(pPngStruct, pPngInfo);
		//png_uint_32	interlaceType	= png_get_interlace_type(pPngStruct, pPngInfo);
		//png_uint_32	compressionType	= png_get_compression_type(pPngStruct, pPngInfo);
		//png_uint_32	filterType	= png_get_filter_type(pPngStruct, pPngInfo);
		

		//possible values for color_type		
		//PNG_COLOR_TYPE_GRAY
		//PNG_COLOR_TYPE_PALETTE
		//PNG_COLOR_TYPE_RGB
		//PNG_COLOR_TYPE_RGBA
		//PNG_COLOR_TYPE_GA
		
		//color type conversion
		switch (colorType)
		{
			case PNG_COLOR_TYPE_PALETTE:
				png_set_palette_to_rgb(pPngStruct);
				channels = 3;
				colorType = PNG_COLOR_TYPE_RGB;
				break;
        

			case PNG_COLOR_TYPE_GA:
			case PNG_COLOR_TYPE_GRAY:
				if(bitDepth < 8)
					png_set_expand_gray_1_2_4_to_8(pPngStruct);
				bitDepth = 8;
				break;
		}
		
		// if the image has a transperancy set.. convert it to a full Alpha channel
		if(png_get_valid(pPngStruct, pPngInfo, PNG_INFO_tRNS))
		{
			png_set_tRNS_to_alpha(pPngStruct);
			++channels;
		}
		
		//TODO: in case we need to downsample
		//if(bitDepth == 16)
		//	png_set_strip_16(pPngStruct);
			
			
		cout << "read PNG file" << endl;
		cout << width << " x " << height << endl;
		cout << bitDepth << " x " << channels << endl;
		cout << colorType << endl;

		
		pvrHeader = CPVRTextureHeader(width, height);
		pvrtexlib::PixelType pixelType = texture::getSupportedPixelType(colorType, bitDepth);
		
		//format supported?
		if((int)pixelType < 0)
			return -1;
		
		pvrHeader.setPixelType(pixelType);
		
		//create PVR texture data
		png_uint_32 imageSize = width * height * bitDepth * channels / 8;
		pvrData = CPVRTextureData((size_t)imageSize);
		
		//decompression
		vector<png_byte*>	rowPtrs(height, NULL);
		const png_uint_32 stride = width * bitDepth * channels / 8;
		
		//set row ptr to starting address
		for(png_uint_32 i = 0; i < height; ++i)
		{
			png_uint_32 q = i * stride;	//(height - i - 1) * stride;
			rowPtrs[i] = (png_byte*)pvrData.getData() + q;
		}
		
		//read image into row ptrs, which is pointing to our data buffer
		png_read_image(pPngStruct, (png_byte**)&rowPtrs[0]);
		png_destroy_read_struct(&pPngStruct, &pPngInfo, &pPngInfo_End);

		int readBytes = (int)ftell(file);
		fclose(file);
		
		return readBytes;
	}
	
	template<>
	int FileHandlerImpl<FileType_PNG>::internal_write(const string& filename, const CPVRTextureHeader& pvrHeader, const CPVRTextureData& pvrData) const
	{
		FILE*	file = fopen(filename.c_str(), "w+b");
		assert(file);
		if(!file)
			return -2;
		
		png_struct* pPngStruct = png_create_write_struct(PNG_LIBPNG_VER_STRING,
														 NULL, //user_error_ptr,
														 NULL, //user_error_fn,
														 NULL //user_warning_fn
														 );
		assert(pPngStruct);
		if(!pPngStruct)
			return -4;
		
		png_info* pPngInfo = png_create_info_struct(pPngStruct);
		assert(pPngInfo);
		if(!pPngInfo)
		{
			png_destroy_write_struct(&pPngStruct, (png_info**)NULL);
			return -5;
		}	
		
		//error handler
		if (setjmp(png_jmpbuf(pPngStruct)))
		{
			png_destroy_write_struct(&pPngStruct, &pPngInfo);
			fclose(file);
			return -6;
		}
		
		png_init_io(pPngStruct, file);
		
		//optional progress handler
		//png_set_write_status_fn(pPngStruct, write_row_callback);
		
		png_set_filter(pPngStruct, 0,
					   PNG_FILTER_NONE  | PNG_FILTER_VALUE_NONE
					   //   |
					   //	   PNG_FILTER_SUB   | PNG_FILTER_VALUE_SUB  |
					   //	   PNG_FILTER_UP    | PNG_FILTER_VALUE_UP   |
					   //	   PNG_FILTER_AVG   | PNG_FILTER_VALUE_AVG  |
					   //	   PNG_FILTER_PAETH | PNG_FILTER_VALUE_PAETH|
					   //	   PNG_ALL_FILTERS
					   );
		
		pvrtexlib::PixelType pvrPixelType = pvrHeader.getPixelType();
		texture::PngFormatInfo pngFormatInfo = texture::getPngFormatInfo(pvrPixelType);

		//format supported?
		if(pngFormatInfo.colorType < 0)
			return -1;
		
		png_uint_32 colorType	= (png_uint_32)pngFormatInfo.colorType;
		png_uint_32	bitDepth	= (png_uint_32)pngFormatInfo.bits;
		png_uint_32 channels	= 0;
		
		switch(colorType)
		{
			case PNG_COLOR_TYPE_GRAY:
				channels = 1;
				break;

			case PNG_COLOR_TYPE_GA:
				channels = 2;
				break;
				
			case PNG_COLOR_TYPE_RGB:
				channels = 3;
				break;
				
			case PNG_COLOR_TYPE_RGBA:
				channels = 4;
				break;
		}

		
		png_set_IHDR(pPngStruct, pPngInfo,
					 pvrHeader.getWidth(), pvrHeader.getHeight(),
					 bitDepth, colorType, 
					 PNG_INTERLACE_NONE,
					 PNG_COMPRESSION_TYPE_DEFAULT,
					 PNG_FILTER_TYPE_DEFAULT);
		

		if (bitDepth > 8)
			png_set_swap(pPngStruct);
		
		vector<png_byte*>	rowPtrs(pvrHeader.getHeight(), NULL);
		const png_uint_32 stride = pvrHeader.getWidth() * bitDepth * channels / 8;

		//set row ptr to starting address
		for(png_uint_32 i = 0; i < pvrHeader.getHeight(); ++i)
		{
			png_uint_32 q = i * stride;	//(pvrHeader.getHeight() - i - 1) * stride;
			rowPtrs[i] = (png_byte*)pvrData.getData() + q;
		}
		
		png_write_info(pPngStruct, pPngInfo);
		png_write_rows(pPngStruct, (png_byte**)&rowPtrs[0],
					   pvrHeader.getHeight());

		int writtenBytes = (int)ftell(file);
		fclose(file);
		
		return writtenBytes;
	}

	
}}

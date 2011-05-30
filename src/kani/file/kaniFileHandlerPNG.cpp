//
//  kaniFileHandlerPNG.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 10.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#include "kaniFileHandlerPNG.h"
#include "../texture/kaniTexPixelFormat.h"
#include <libpng/png.h>

#include <pvrtex/CPVRTexture.h>
#include <pvrtex/CPVRTextureHeader.h>
#include <pvrtex/CPVRTextureData.h>

#include <iostream>
#include <cstdio>
#include <cassert>


#define PNGSIGSIZE 8


namespace kani { namespace file {
	
	//ref: @see http://www.piko3d.com/tutorials/libpng-tutorial-loading-png-files-from-streams
	//ref: @see http://zarb.org/~gc/html/libpng.html
	
	using std::ifstream;
	using std::ios_base;
	using std::cout;
	using std::cerr;
	using std::endl;
	
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
		

		png_byte**	rowPtrs = NULL;
		char*		data = NULL;

		
		
		//error handling
		if(setjmp(png_jmpbuf(pPngStruct)))
		{
			png_destroy_read_struct(&pPngStruct, &pPngInfo, &pPngInfo_End);
			fclose(file);
			
			if (rowPtrs != NULL)
				delete [] rowPtrs;
			
			if (data != NULL)
				delete [] data;
			
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
		png_uint_32	interlaceType	= png_get_interlace_type(pPngStruct, pPngInfo);
		png_uint_32	compressionType	= png_get_compression_type(pPngStruct, pPngInfo);
		png_uint_32	filterType	= png_get_filter_type(pPngStruct, pPngInfo);
		

		//possible values for color_type
		//TODO: add to enum
		//TODO: in case of PALETTE: transform to RGB (A)
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
				if (bitDepth < 8)
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
		pvrHeader.setPixelType(texture::getSupportedPixelType(colorType, bitDepth));
		
		//create PVR texture data
		png_uint_32 imageSize = width * height * bitDepth * channels / 8;
		pvrData = CPVRTextureData((size_t)imageSize);
		
		//decompression
		rowPtrs	= new png_byte*[height];
		const png_uint_32 stride = width * bitDepth * channels / 8;
		
		for(png_uint_32 i = 0; i < height; ++i)
		{
			rowPtrs[i] = NULL;
		}
		
		//set row ptr to starting address
		for(png_uint_32 i = 0; i < height; ++i)
		{
			png_uint_32 q = (height - i - 1) * stride;
			rowPtrs[i] = (png_byte*)pvrData.getData() + q;
		}
		
		//read image into row ptrs, which is pointing to our data buffer
		png_read_image(pPngStruct, rowPtrs);


		//Delete the row pointers array....
		delete[] rowPtrs;		
		
		//And don't forget to clean up the read and info structs !
		png_destroy_read_struct(&pPngStruct, &pPngInfo, &pPngInfo_End);

		int readBytes = (int)ftell(file);
		fclose(file);
		
		return readBytes;
		
	}
	
	template<>
	int FileHandlerImpl<FileType_PNG>::internal_write(const string& filename, const CPVRTextureHeader&, const CPVRTextureData&) const
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

		
		return -3;
	}

	
}}

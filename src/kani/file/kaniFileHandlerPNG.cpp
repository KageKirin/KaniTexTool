//
//  kaniFileHandlerPNG.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 10.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#include "kaniFileHandlerPNG.h"
#include "../texture/kaniTexFormat.h"
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
			png_destroy_read_struct(&pPngStruct, (png_infopp)NULL, (png_infopp)NULL);
			return -5;
		}

		png_info* pPngInfo_End = png_create_info_struct(pPngStruct);
		if(!pPngInfo_End)
		{
			png_destroy_read_struct(&pPngStruct, &pPngInfo, (png_infopp)NULL);
			return -5;
		}
		

		png_bytep*	rowPtrs = NULL;
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
		//PNG_COLOR_TYPE_RGB_ALPHA
		//PNG_COLOR_TYPE_GRAY_ALPHA
		
		switch (colorType)
		{
        case PNG_COLOR_TYPE_PALETTE:
            png_set_palette_to_rgb(pPngStruct);
            //Don't forget to update the channel info (thanks Tom!)
            //It's used later to know how big a buffer we need for the image
            channels = 3;           
            break;
        
		case PNG_COLOR_TYPE_GRAY:
            if (bitDepth < 8)
				png_set_expand_gray_1_2_4_to_8(pPngStruct);
            //And the bitdepth info
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
		
		/*	
		//Here's one of the pointers we've defined in the error handler section:
		//Array of row pointers. One for every row.
		rowPtrs = new png_bytep[imgHeight];

		//Alocate a buffer with enough space.
		//(Don't use the stack, these blocks get big easilly)
		//This pointer was also defined in the error handling section, so we can clean it up on error.
		data = new char[imgWidth * imgHeight * bitdepth * channels / 8];
		//This is the length in bytes, of one row.
		const unsigned int stride = imgWidth * bitdepth * channels / 8;

		//A little for-loop here to set all the row pointers to the starting
		//Adresses for every row in the buffer

		for (size_t i = 0; i < imgHeight; i++) 
		{
			//Set the pointer to the data pointer + i times the row stride.
			//Notice that the row order is reversed with q.
			//This is how at least OpenGL expects it,
			//and how many other image loaders present the data.
			uint64 q = (imgHeight- i - 1) * stride;
			rowPtrs[i] = (png_bytep)data + q;
		}

		//And here it is! The actuall reading of the image!
		//Read the imagedata and write it to the adresses pointed to
		//by rowptrs (in other words: our image databuffer)
		png_read_image(pngPtr, rowPtrs);


		//Delete the row pointers array....
		delete[] (png_bytep)rowPtrs;
		
		
		//And don't forget to clean up the read and info structs !
		png_destroy_read_struct(&pngPtr, &infoPtr,(png_infopp)0);
		*/
		

		return -3;
		
	}
	
	template<>
	int FileHandlerImpl<FileType_PNG>::internal_write(const string& filename, const CPVRTextureHeader&, const CPVRTextureData&) const
	{
		return -3;
	}

	
}}

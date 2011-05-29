//
//  kaniFileHandlerPNG.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 10.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#include "kaniFileHandlerPNG.h"
#include <libpng/png.h>

#include <fstream>
#include <iostream>
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
	
	void userReadData(png_structp pngPtr, png_bytep data, png_size_t length)
	{
		//Here we get our IO pointer back from the read struct.
		//This is the parameter we passed to the png_set_read_fn() function.
		//Our std::istream pointer.
		png_voidp a = png_get_io_ptr(pngPtr);
		//Cast the pointer to std::istream* and read 'length' bytes into 'data'
		((std::istream*)a)->read((char*)data, length);
	}
	
	template<>
	int FileHandlerImpl<FileType_PNG>::internal_read(const string& filename, CPVRTextureHeader&, CPVRTextureData&) const
	{		
		ifstream file(filename.c_str(), ios_base::in | ios_base::binary);
		if(!file.is_open())
			return -2;
			
		file.seekg(0, ios_base::end);
		size_t fileSize = file.tellg();
		file.seekg (0, ios_base::beg);
	
		//check if png
		png_byte pngsig[PNGSIGSIZE];
		file.read((char*)pngsig, sizeof(pngsig));
		
		int isPng = png_sig_cmp(pngsig, 0, PNGSIGSIZE);
		assert(isPng == 0);
		if(isPng != 0)
			return -3;
	
		png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		assert(pngPtr);
		if(!pngPtr)
			return -4;
			
		png_infop infoPtr = png_create_info_struct(pngPtr);
		assert(infoPtr);
		if(!infoPtr)
		{
		    png_destroy_read_struct(&pngPtr, (png_infopp)0, (png_infopp)0);
			return -5;
		}
		
		//Here I've defined 2 pointers up front, so I can use them in error handling.
		//I will explain these 2 later. Just making sure these get deleted on error.
		png_bytep* rowPtrs = NULL;
		char* data = NULL;

		if (setjmp(png_jmpbuf(pngPtr)))
		{
			//An error occured, so clean up what we have allocated so far...
			png_destroy_read_struct(&pngPtr, &infoPtr,(png_infopp)0);
			if (rowPtrs != NULL)
				delete [] rowPtrs;
			
			if (data != NULL)
				delete [] data;

			cout << "ERROR: An error occured while reading the PNG file" << endl;

			//Make sure you return here. libPNG will jump to here if something
			//goes wrong, and if you continue with your normal code, you might
			//End up with an infinite loop.
			return -6; // Do your own error handling here.
		}
		
		
		png_set_read_fn(pngPtr,(voidp)&file, userReadData);
		
		
		//Set the amount signature bytes we've already read:
		//We've defined PNGSIGSIZE as 8;
		png_set_sig_bytes(pngPtr, PNGSIGSIZE);

		//Now call png_read_info with our pngPtr as image handle, and infoPtr to receive the file info.
		png_read_info(pngPtr, infoPtr);
		
		
		png_uint_32 imgWidth =  png_get_image_width(pngPtr, infoPtr);
		png_uint_32 imgHeight = png_get_image_height(pngPtr, infoPtr);

		//bits per CHANNEL! note: not per pixel!
		png_uint_32 bitdepth   = png_get_bit_depth(pngPtr, infoPtr);

		//Number of channels
		png_uint_32 channels   = png_get_channels(pngPtr, infoPtr);

		//Color type. (RGB, RGBA, Luminance, luminance alpha... palette... etc)
		png_uint_32 color_type = png_get_color_type(pngPtr, infoPtr);

		
		//possible values for color_type
		//TODO: add to enum
		//TODO: in case of PALETTE: transform to RGB (A)
		//PNG_COLOR_TYPE_GRAY
		//PNG_COLOR_TYPE_PALETTE
		//PNG_COLOR_TYPE_RGB
		//PNG_COLOR_TYPE_RGB_ALPHA
		//PNG_COLOR_TYPE_GRAY_ALPHA
		
		switch (color_type)
		{
        case PNG_COLOR_TYPE_PALETTE:
            png_set_palette_to_rgb(pngPtr);
            //Don't forget to update the channel info (thanks Tom!)
            //It's used later to know how big a buffer we need for the image
            channels = 3;           
            break;
        
		case PNG_COLOR_TYPE_GRAY:
            if (bitdepth < 8)
				png_set_expand_gray_1_2_4_to_8(pngPtr);
            //And the bitdepth info
            bitdepth = 8;
            break;
		}
		
		// if the image has a transperancy set.. convert it to a full Alpha channel
		if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS))
		{
			png_set_tRNS_to_alpha(pngPtr);
			channels+=1;
		}
		
		//TODO: in case we need to downsample
		//We don't support 16 bit precision.. so if the image Has 16 bits per channel
        //precision... round it down to 8.
		if (bitdepth == 16)
			png_set_strip_16(pngPtr);
			
			
		cout << "read PNG file" << endl;
		cout << imgWidth << " x " << imgHeight << endl;
		cout << bitdepth << " x " << channels << endl;
		cout << color_type << endl;
			
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
		
		

		return -3;
		
	}
	
	template<>
	int FileHandlerImpl<FileType_PNG>::internal_write(const string& filename, const CPVRTextureHeader&, const CPVRTextureData&) const
	{
		return -3;
	}

	
}}

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
#include <cassert>

#define PNGSIGSIZE 8


namespace kani { namespace file {
	
	//ref: @see http://www.piko3d.com/tutorials/libpng-tutorial-loading-png-files-from-streams
	//ref: @see http://zarb.org/~gc/html/libpng.html
	
	using std::ifstream;
	using std::ios_base;
	
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
		


		return -3;
		
	}
	
	template<>
	int FileHandlerImpl<FileType_PNG>::internal_write(const string& filename, const CPVRTextureHeader&, const CPVRTextureData&) const
	{
		return -3;
	}

	
}}

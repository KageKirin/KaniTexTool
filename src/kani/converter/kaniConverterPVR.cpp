//
//  kaniConverterPVR.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 20.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

//#include "stdafx.h"
#include "kaniConverterPVR.h"
#include "../file/kaniFileTypes.h"
#include "../file/kaniFileHandler.h"
#include "../texture/kaniTexPixelFormat.h"

#include <pvrtex/PVRTexLib.h>
#include <pvrtex/CPVRTexture.h>
#include <pvrtex/PVRTexLibGlobals.h>
#include <iostream>


namespace kani { namespace converter {

	using std::cout;
	using std::cerr;
	using std::endl;
	using kani::file::FileHandler;
	using pvrtexlib::PixelType;
	using pvrtexlib::PVRTextureUtilities;


	bool ConverterPVR::internal_convert(const string&	inputFilename,
										const string&	outputFilename,
										const string&	format,
										int				mipmaps,
										bool			regenMips) const
	{
		// get the utilities instance
		PVRTextureUtilities sPVRU = PVRTextureUtilities();
		
		pvrtexlib::CPVRTextureHeader	texHeader_Orig;
		pvrtexlib::CPVRTextureData		texData_Orig;

		cout << "reading from " << inputFilename << endl;		
		int inputDataSize = FileHandler::read(inputFilename, texHeader_Orig, texData_Orig);
		cout << "read " << inputDataSize << " bytes" << endl;
		if(inputDataSize <= 0)
			return false;
		
	//	cout << texHeader_Orig.getWidth() << " x " << texHeader_Orig.getHeight() << endl;
	//	cout << texHeader_Orig.getPixelType()	<< endl;
	//	cout << texHeader_Orig.getNumSurfaces()	<< endl;
	//	cout << texHeader_Orig.getMipMapCount()	<< endl;	
	//	for(kani::uint32 ml = 0; ml < texHeader_Orig.getMipMapCount(); ++ml)
	//	{
	//		cout << "mip " << ml << texHeader_Orig.getMipWidth(ml) << " x " << texHeader_Orig.getMipHeight(ml) << endl;
	//	}


		int outputDataSize = 0;
		PixelType pt = kani::texture::getSupportedPixelType(format.c_str());
		if(pt == pvrtexlib::MGLPT_NOTYPE)
		{
			pt = texHeader_Orig.getPixelType();
		}
		
		//check if any conversion needed
		if((int)pt < 0 && texHeader_Orig.getMipMapCount() == mipmaps && !regenMips)
		{
			outputDataSize = FileHandler::write(outputFilename, texHeader_Orig, texData_Orig);
		}
		else
		{
			//decompression
			cout << "decompressing" << endl;
			pvrtexlib::CPVRTextureHeader	texHeader_Dec(texHeader_Orig);
			texHeader_Dec.setPixelType(pvrtexlib::MGLPT_ARGB_8888);
			pvrtexlib::CPVRTextureData		texData_Dec;
			sPVRU.DecompressPVR(texHeader_Orig, texData_Orig,texHeader_Dec, texData_Dec);
			
		
			//process
			if(regenMips && mipmaps < 0)
			{
				mipmaps = texHeader_Orig.getMipMapCount();
			}
			if(mipmaps >= 0)
			{
				cout << "processing" << endl;
				pvrtexlib::CPVRTextureHeader	texHeader_Proc(texHeader_Dec);
				texHeader_Proc.setMipMapCount(mipmaps);
				texHeader_Proc.setFalseMips(false);
				sPVRU.ProcessRawPVR(texHeader_Dec, texData_Dec, texHeader_Proc);
			}
			
			//compression
			cout << "compressing" << endl;
			pvrtexlib::CPVRTextureHeader	texHeader_Out(texHeader_Orig);
			texHeader_Out.setPixelType(pt);
			pvrtexlib::CPVRTextureData		texData_Out;
			sPVRU.CompressPVR(texHeader_Dec, texData_Dec, texHeader_Out, texData_Out);
			
			
			//write
			cout << "writing to " << outputFilename << endl;
			outputDataSize = FileHandler::write(outputFilename, texHeader_Out, texData_Out);
		}
		
	
		 
		cout << "wrote " << outputDataSize << " bytes" << endl;
		if(outputDataSize <= 0)
			return false;

		return true;
	}

}}

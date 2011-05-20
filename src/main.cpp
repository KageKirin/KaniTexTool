//
//  main.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 08.05.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <string>
#include "kani/file/kaniFileTypes.h"
#include "kani/file/kaniFileHandler.h"
#include "kani/core/kaniCmdLine.h"
#include "kani/kaniConversion.h"

#include <pvrtex/CPVRTexture.h>

int main (int argc, const char** argv)
{
	if(argc <= 1)
	{
		std::cerr << "run with --help for more info" << std::endl;
		return 1;
	}

	kani::cmd::CmdLineOptions options(argc, argv);
	if(options.getState())
	{
		return 1;
	}
	
	std::cout << "convert " << options.getInputFile() 
		<< " to " << options.getOutputFile() 
		<< " with " << options.getFormat()
		<< " mips " << options.getMipmaps() << std::endl;
		
	pvrtexlib::CPVRTextureHeader	texHeader;
	pvrtexlib::CPVRTextureData		texData;

	std::cout << "read " << kani::file::FileHandler::read(options.getInputFile(), texHeader, texData) << " bytes" << std::endl;
	
	std::cout << texHeader.getWidth()	<< std::endl;
	std::cout << texHeader.getHeight()	<< std::endl;

	kani::uint32 w, h;
	texHeader.getDimensions(w, h);
	std::cout << w << " x " << h		<< std::endl;

	std::cout << texHeader.getPixelType()	<< std::endl;
	std::cout << texHeader.getNumSurfaces()	<< std::endl;
	std::cout << texHeader.getMipMapCount()	<< std::endl;
	
	for(kani::uint32 ml = 0; ml < texHeader.getMipMapCount(); ++ml)
	{
		std::cout << "mip " << ml << texHeader.getMipWidth(ml) << " x " << texHeader.getMipHeight(ml) << std::endl;
	}

	std::cout << "wrote " << kani::file::FileHandler::write(options.getOutputFile(), texHeader, texData) << " bytes" << std::endl;

	
	//	kani::convert(options.getInputFile(),
	//			  options.getOutputFile(),
	//			  options.getFormat(),
	//			  options.getMipmaps());
	
	std::cout << std::hex << kani::file::getFileTypeFromFilename(options.getInputFile()) << std::endl;
	std::cout << std::hex << kani::file::getFileTypeFromFilename(options.getOutputFile()) << std::endl;
	
	std::cout << std::hex << kani::file::FileType_DDS << std::endl;
	std::cout << std::hex << kani::file::FileType_KTX << std::endl;
	std::cout << std::hex << kani::file::FileType_PNG << std::endl;
	std::cout << std::hex << kani::file::FileType_PVR << std::endl;
    return 0;
}


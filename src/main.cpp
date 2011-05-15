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
#include "kani/core/kaniCmdLine.h"
#include "kani/kaniConversion.h"

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
	
	//	kani::convert(options.getInputFile(),
	//			  options.getOutputFile(),
	//			  options.getFormat(),
	//			  options.getMipmaps());
	
	std::cout << std::hex << kani::filetypes::getFileTypeFromFilename(options.getInputFile()) << std::endl;
	std::cout << std::hex << kani::filetypes::getFileTypeFromFilename(options.getOutputFile()) << std::endl;
	
	std::cout << std::hex << kani::filetypes::FileType_DDS << std::endl;
	std::cout << std::hex << kani::filetypes::FileType_KTX << std::endl;
	std::cout << std::hex << kani::filetypes::FileType_PNG << std::endl;
	std::cout << std::hex << kani::filetypes::FileType_PVR << std::endl;
    return 0;
}


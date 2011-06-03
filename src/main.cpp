//
//  main.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 08.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "kani/core/kaniCmdLine.h"
#include "kani/converter/kaniConverter.h"

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
	
	std::cout
		<< "convert "	<< options.getInputFile()
		<< " to "		<< options.getOutputFile()
		<< " with "		<< options.getFormat()
		<< " mips "		<< options.getMipmaps()
		<< std::endl;
	
	bool result = kani::converter::Converter::convert(
		options.getConverter(),
		options.getInputFile(),
		options.getOutputFile(),
		options.getFormat(),
		options.getMipmaps(),
		options.getRegenMips()
		);
	
    return result ? 0 : 1;
}


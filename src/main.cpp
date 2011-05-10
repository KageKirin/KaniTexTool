//
//  main.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 08.05.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "kani/file/kaniFileTypes.h"

int main (int argc, const char * argv[])
{

	// insert code here...
	std::cout << std::hex << kani::filetypes::FileDDS << "\n";
	std::cout << std::hex << kani::filetypes::FileKTX << "\n";
	std::cout << std::hex << kani::filetypes::FilePNG << "\n";
	std::cout << std::hex << kani::filetypes::FilePVR << "\n";
    return 0;
}


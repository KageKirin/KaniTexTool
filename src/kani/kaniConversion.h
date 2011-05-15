//
//  kaniConversion.h
//  KaniTexTool
//
//  Created by Christian Helmich on 15.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#pragma once
#ifndef KANI_CONVERSION_H
#define KANI_CONVERSION_H

#include "./texture/kaniTexFormat.h"
#include <string>

namespace kani
{
	using std::string;
	using kani::texture::Format;
	
	bool convert(const string& inputFilename,
				 const string& outputFilename,
				 Format format,
				 int mipmaps);
}

#endif	//KANI_CONVERSION_H
//
//  kaniConverter.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 20.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

//#include "stdafx.h"
#include "kaniConverter.h"
#include "kaniConverterPVR.h"

namespace kani { namespace converter {


	//----------------------------------------------------------------------
	
	bool Converter::convert(const string& converter,
							const string& inputFilename,
							const string& outputFilename,
							const string& format,
							int		mipmaps,
							bool	regenMips)
	{
		return getConverter(converter).internal_convert(inputFilename,
														outputFilename,
														format,
														mipmaps,
														regenMips);
	}							
	
	//TODO: use as strategy pattern entry later
	Converter& Converter::getConverter(const string& converter)
	{
		static ConverterPVR	cPVR;
		return cPVR;
	}
	
	//----------------------------------------------------------------------	
				
	bool Converter::internal_convert(const string&	inputFilename,
									 const string&	outputFilename,
									 const string&	format,
									 int			mipmaps,
									 bool			regenMips) const
	{
		return false;
	}

	//----------------------------------------------------------------------
	
}}

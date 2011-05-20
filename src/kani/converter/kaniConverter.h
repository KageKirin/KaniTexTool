//
//  kaniConverter.h
//  KaniTexTool
//
//  Created by Christian Helmich on 20.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#pragma once
#ifndef	KANI_CONVERTER_H
#define	KANI_CONVERTER_H	1

#include <string>

namespace kani { namespace converter {
	
	using std::string;
		
	class Converter
	{
	public:

		static bool convert(const string& converter,
							const string& inputFilename,
							const string& outputFilename,
							const string& format,
							int		mipmaps,
							bool	regenMips);
							
	protected:
		static Converter& getConverter(const string& converter);
		
		virtual bool internal_convert(	const string&	inputFilename,
										const string&	outputFilename,
										const string&	format,
										int				mipmaps,
										bool			regenMips) const;
	};
	
}}

#endif	//KANI_CONVERTER_H
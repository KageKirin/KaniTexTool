//
//  kaniConverterPVR.h
//  KaniTexTool
//
//  Created by Christian Helmich on 20.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#pragma once
#ifndef KANI_CONVERTER_PVR_H
#define KANI_CONVERTER_PVR_H	1

#include "kaniConverter.h"

namespace kani { namespace converter {

	class ConverterPVR : public Converter
	{
	protected:
		virtual bool internal_convert(const string&	inputFilename,
							 const string&	outputFilename,
							 const string&	format,
							 int			mipmaps,
							 bool			regenMips) const;
	};

}}

#endif	//KANI_CONERTER_PVR_H
//
//  kaniUtils.h
//  KaniTexTool
//
//  Created by Christian Helmich on 09.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#pragma once
#ifndef KANI_UTILS_H
#define KANI_UTILS_H 1

#include "kaniTypes.h"

namespace kani
{
	
	namespace endian
	{
		template<typename T>
		struct static_swap
		{
			template<T v>
			struct swap;		
		};
	
		template<>
		struct static_swap<uint8>
		{
			template<uint8 v>
			struct swap
			{
				static const uint8 value = v;
			};
		};
	
		template<>
		struct static_swap<uint16>
		{
			template<uint16 v>
			struct split
			{
				static const uint8 upper_value = (v & 0xff00) >> 8;
				static const uint8 lower_value = (v & 0x00ff);
			};
		
		
		
			template<uint16 v>
			struct swap
			{
				static const uint16 value = 
					(static_swap<uint8>::swap<split<v>::lower_value>::value << 8) |
					(static_swap<uint8>::swap<split<v>::upper_value>::value);
			};	
		};
	
		template<>
		struct static_swap<uint32>
		{
			template<uint32 v>
			struct split
			{
				static const uint16	upper_value = (v & 0xffff0000) >> 16;
				static const uint16	lower_value = (v & 0x0000ffff);
			};
		
		
			template<uint32 v>
			struct swap
			{
				static const uint32 value = 
					(static_swap<uint16>::swap<split<v>::lower_value>::value << 16) |
					(static_swap<uint16>::swap<split<v>::upper_value>::value);
			};
		};
	
		template<>
		struct static_swap<uint64>
		{
			template<uint64 v>
			struct split
			{
				static const uint32	upper_value = (v & 0xffffffff00000000) >> 32;
				static const uint32	lower_value = (v & 0x00000000ffffffff);
			};
		
		
			template<uint64 v>
			struct swap
			{
				static const uint64 value = 
					(static_swap<uint32>::swap<split<v>::lower_value>::value  << 32) |
					(static_swap<uint32>::swap<split<v>::upper_value>::value);
			};
		};
	}//ns endian
	
}//ns kani

#endif	//KANI_UTILS_H
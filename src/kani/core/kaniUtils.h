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
	template<typename T>
	struct static_swap
	{
		template<T v>
		struct swap;		
	};
	
	template<uint8 v>
	struct static_swap<uint8>::swap<v>
	{
		static const uint8 value = v;
	};
	
	template<uint16 v>
	struct static_swap<uint16>::swap<v>
	{
		static const uint16 value = 
			static_swap<uint8>::swap<(v & 0x00ff)>::value << 8 |
			static_swap<uint8>::swap<((v & 0xff00) >> 8)>;
	};
	
	template<uint32 v>
	struct static_swap<uint32>::swap<v>
	{
		static const uint32 value = 
			static_swap<uint16>::swap<(v & 0x0000ffff)>::value << 16 |
			static_swap<uint16>::swap<((v & 0xffff0000) >> 16)>;
	};

	template<uint64 v>
	struct static_swap<uint64>::swap<v>
	{
		static const uint64 value = 
			static_swap<uint32>::swap<(v & 0x00000000ffffffff)>::value << 32 |
			static_swap<uint32>::swap<((v & 0xffffffff00000000) >> 32)>;
	};

}

#endif	//KANI_UTILS_H
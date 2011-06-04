//
//  kaniTexSize.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 6/1/11.
//  Copyright 2011 KageKirin. All rights reserved.
//

//#include "stdafx.h"
#include "kaniTexSize.h"
#include "kaniTexFourCC.h"

#include <pvrtex/CPVRTextureHeader.h>
#include <map>
#include <vector>
#include <algorithm>

namespace kani { namespace texture {

	using std::map;
	using std::vector;
	using std::find_if;
	using std::max;
	using std::min;
	
	//----------------------------------------------------------------------

	int	TextureSize::operator()(int width, int height, int mipLevel, bool padding) const
	{
		return -1;
	}
	
	int	TextureSize::operator()(int width, int height, int depth, int mipLevel, bool padding) const
	{
		return -1;	
	}
	
	
	static TextureSize	s_DefaultTextureSize;
	
	//----------------------------------------------------------------------

	template<FourCC fcc>
	struct	TextureSizeImpl : TextureSize
	{
		virtual	int	operator()(int width, int height, int mipLevel = 0, bool padding = false) const
		{
			return -2;
		}
		
		virtual	int	operator()(int width, int height, int depth, int mipLevel = 0, bool padding = false) const
		{
			int mipDepth	= max(depth		>> mipLevel, 1);
			return (*this)(width, height, mipLevel, padding) * mipDepth;
		}
	};
	
	//----------------------------------------------------------------------
	//TODO: implement template specialization for all FourCCs
	//TODO: DXT5 -> DXT3 -> DXT1

	template<>
	int	TextureSizeImpl<FourCC_DXT1>::operator()(int width, int height, int mipLevel, bool padding) const
	{	
		int mipWidth	= max(width		>> mipLevel, 1);
		int mipHeight	= max(height	>> mipLevel, 1);
		
		int blockcount = ((mipWidth + 3) / 4) * ((mipHeight + 3) / 4);
		int blocksize = 8;
		return blockcount * blocksize;		
	}

	template<>
	int	TextureSizeImpl<FourCC_DXT2>::operator()(int width, int height, int mipLevel, bool padding) const
	{	
		int mipWidth	= max(width		>> mipLevel, 1);
		int mipHeight	= max(height	>> mipLevel, 1);

		int blockcount = ((mipWidth + 3) / 4) * ((mipHeight + 3) / 4);
		int blocksize = 16;
		return blockcount * blocksize;
	}

	template<>
	int	TextureSizeImpl<FourCC_DXT3>::operator()(int width, int height, int mipLevel, bool padding) const
	{	
		int mipWidth	= max(width		>> mipLevel, 1);
		int mipHeight	= max(height	>> mipLevel, 1);

		int blockcount = ((mipWidth + 3) / 4) * ((mipHeight + 3) / 4);
		int blocksize = 16;
		return blockcount * blocksize;
	}

	template<>
	int	TextureSizeImpl<FourCC_DXT4>::operator()(int width, int height, int mipLevel, bool padding) const
	{	
		int mipWidth	= max(width		>> mipLevel, 1);
		int mipHeight	= max(height	>> mipLevel, 1);

		int blockcount = ((mipWidth + 3) / 4) * ((mipHeight + 3) / 4);
		int blocksize = 16;
		return blockcount * blocksize;
	}

	template<>
	int	TextureSizeImpl<FourCC_DXT5>::operator()(int width, int height, int mipLevel, bool padding) const
	{	
		int mipWidth	= max(width		>> mipLevel, 1);
		int mipHeight	= max(height	>> mipLevel, 1);

		int blockcount = ((mipWidth + 3) / 4) * ((mipHeight + 3) / 4);
		int blocksize = 16;
		return blockcount * blocksize;
	}

	
	//----------------------------------------------------------------------

	template<>
	int	TextureSizeImpl<FourCC_PVR2>::operator()(int width, int height, int mipLevel, bool padding) const
	{	
		int mipWidth	= max(width		>> mipLevel, 1);
		int mipHeight	= max(height	>> mipLevel, 1);

		return max(mipWidth * mipHeight / 2, 32);
	}

	template<>
	int	TextureSizeImpl<FourCC_PVR4>::operator()(int width, int height, int mipLevel, bool padding) const
	{	
		int mipWidth	= max(width		>> mipLevel, 1);
		int mipHeight	= max(height	>> mipLevel, 1);

		return max(mipWidth * mipHeight / 2, 32);
	}
	
	
	
	template<>
	int	TextureSizeImpl<FourCC_ETC1>::operator()(int width, int height, int mipLevel, bool padding) const
	{	
		int mipWidth	= max(width		>> mipLevel, 1);
		int mipHeight	= max(height	>> mipLevel, 1);
		
		pvrtexlib::CPVRTextureHeader pvrHeader(mipWidth, mipHeight);
		pvrHeader.setPixelType(pvrtexlib::ETC_RGB_4BPP);
		return (int)pvrHeader.getSurfaceSize();
	}
	
	//----------------------------------------------------------------------

	typedef map<FourCC, TextureSize*>	FourCCTextureSizeMap;

#define FOURCC_NAME(elem)			BOOST_PP_CAT(FourCC_, BOOST_PP_TUPLE_ELEM(2, 0, elem))
#define FOURCC_IMPL(data, elem)		BOOST_PP_CAT(data, BOOST_PP_TUPLE_ELEM(2, 0, elem))

#define M_CREATE_FOURCCMAP(r, data, elem)									\
	static TextureSizeImpl< FOURCC_NAME(elem) >	FOURCC_IMPL(data, elem);	\
	s_FourCCTextureSizeMap[ FOURCC_NAME(elem) ]	= & FOURCC_IMPL(data, elem);\

	

	FourCCTextureSizeMap&	getFourCCTextureSizeMap()
	{
		static	FourCCTextureSizeMap		s_FourCCTextureSizeMap;
		
		static bool initDone = false;
		if(!initDone)
		{
			s_FourCCTextureSizeMap.clear();
			BOOST_PP_SEQ_FOR_EACH(M_CREATE_FOURCCMAP, s_TextureSize_FourCC_, FOURCC_SEQ)
			initDone = true;
		}		
		
		return s_FourCCTextureSizeMap;
	}


	
	const TextureSize& TextureSize::getTextureSize(FourCC fourCC)
	{
		FourCCTextureSizeMap& fourCCTextureSizeMap = getFourCCTextureSizeMap();
		
		FourCCTextureSizeMap::iterator iter = fourCCTextureSizeMap.find(fourCC);
		if(iter != fourCCTextureSizeMap.end())
			return *(iter->second);
		
		return s_DefaultTextureSize;		
	}

	//----------------------------------------------------------------------
	
	struct ChannelInfo
	{
		int channels;
		int bits;		
	};
	
	struct	TextureSizeUncompressed : TextureSize
	{
		ChannelInfo	channelInfo;
		
		TextureSizeUncompressed(ChannelInfo& ci):
		channelInfo(ci)
		{}
	
	
		virtual	int	operator()(int width, int height, int mipLevel = 0, bool padding = false) const
		{
			int mipWidth	= width		>> mipLevel;
			int mipHeight	= height	>> mipLevel;
			
			return mipWidth * mipHeight * channelInfo.channels * channelInfo.bits;
		}
		
		virtual	int	operator()(int width, int height, int depth, int mipLevel = 0, bool padding = false) const
		{
			int mipDepth	= depth		>> mipLevel;
			return (*this)(width, height, mipLevel, padding) * mipDepth;
		}
	};
	
	
	typedef vector<TextureSizeUncompressed>		ChannelInfoTextureSizeMap;
		
	struct Pred_ChannelInfo
	{
		int channels;
		int bits;
		
		Pred_ChannelInfo(int c, int b):channels(c),bits(b){}
		
		bool operator()(TextureSizeUncompressed& val)
		{
			return	(val.channelInfo.channels == channels)
				&&	(val.channelInfo.bits == bits);
		}
	};
	
	ChannelInfoTextureSizeMap&	getChannelInfoTextureSizeMap()
	{
		static	ChannelInfoTextureSizeMap		s_ChannelInfoTextureSizeMap;
		
		static bool initDone = false;
		if(!initDone)
		{
			s_ChannelInfoTextureSizeMap.clear();
			ChannelInfo	channelInfo;

			for(channelInfo.channels = 1; channelInfo.channels <= 4; ++channelInfo.channels)
			{
				for(channelInfo.bits = 1; channelInfo.bits <= 32; channelInfo.bits <<= 1)
				{
					s_ChannelInfoTextureSizeMap.push_back(TextureSizeUncompressed(channelInfo));
				}
			}
			initDone = true;
		}		
		
		return s_ChannelInfoTextureSizeMap;
	}

	
	const TextureSize& TextureSize::getTextureSize(int channels, int bits)
	{
		ChannelInfoTextureSizeMap&	channelInfoTextureSizeMap = getChannelInfoTextureSizeMap();

		ChannelInfoTextureSizeMap::iterator iter = find_if(	channelInfoTextureSizeMap.begin(),
															channelInfoTextureSizeMap.end(),
															Pred_ChannelInfo(channels, bits));
		if(iter != channelInfoTextureSizeMap.end())
			return *iter;

		return	s_DefaultTextureSize;
	}
	
	
}}

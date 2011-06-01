//
//  kaniTexSize.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 6/1/11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#include "kaniTexSize.h"

#include "kaniTexFourCC.h"
#include <map>
#include <algorithm>

namespace kani { namespace texture {

	using std::map;
	using std::find_if;
	
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
		{	return -2;	}
		virtual	int	operator()(int width, int height, int depth, int mipLevel = 0, bool padding = false) const
		{	return -2;	}
	};
	
	//----------------------------------------------------------------------

	template<>
	int	TextureSizeImpl<FourCC_DXT1>::operator()(int width, int height, int mipLevel, bool padding) const
	{	return -2;	}

	template<>
	int	TextureSizeImpl<FourCC_DXT1>::operator()(int width, int height, int depth, int mipLevel, bool padding) const
	{	return -2;	}
	
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
	
	
	typedef map<ChannelInfo, TextureSize*>	ChannelInfoTextureSizeMap;
	static	ChannelInfoTextureSizeMap		s_ChannelInfoTextureSizeMap;
	
	struct Pred_ChannelInfo
	{
		int channels;
		int bits;
		
		Pred_ChannelInfo(int c, int b):channels(c),bits(b){}
		
		bool operator()(ChannelInfoTextureSizeMap::value_type& val)
		{
			return	(val.first.channels == channels)
				&&	(val.first.bits == bits);
		}
	};
	
	const TextureSize& TextureSize::getTextureSize(int channels, int bits)
	{
		ChannelInfoTextureSizeMap::iterator iter = find_if(s_ChannelInfoTextureSizeMap.begin(),
													  s_ChannelInfoTextureSizeMap.end(),
													  Pred_ChannelInfo(channels, bits));
		if(iter != s_ChannelInfoTextureSizeMap.end())
			return *(iter->second);

		return	s_DefaultTextureSize;
	}
	
	
}}

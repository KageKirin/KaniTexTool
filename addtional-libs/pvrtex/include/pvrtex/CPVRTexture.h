/******************************************************************************

 @File         CPVRTexture.h

 @Title        Console Log

 @Version      

 @Copyright    Copyright (C)  Imagination Technologies Limited. All Rights Reserved. Strictly Confidential.

 @Platform     ANSI

 @Description  Class that holds data and descriptive information for PVR textures.

******************************************************************************/

#ifndef CPVRTEXTURE_H
#define CPVRTEXTURE_H

#include "CPVRTextureHeader.h"
#include "CPVRTextureData.h"

namespace pvrtexlib
{
	
#ifdef __APPLE__
	/* The classes below are exported */
#pragma GCC visibility push(default)
#endif

class PVR_DLL CPVRTexture
{
public:
	/*******************************************************************************
	* Function Name  : CPVRTexture Constructors
	* Description    :  default
	*					from CPVRTextureHeader but no data
	*					from CPVRTextureHeader/CPVRTextureData pair
	*					from Header with blank data
	*					create Header for raw data
	*					from file
	*					from PVR data with embedded header (contents of PVR file)
	*******************************************************************************/
	CPVRTexture();								// default constructor
	CPVRTexture(CPVRTextureHeader& sHeader);	// from CPVRTextureHeader but no data
	CPVRTexture(CPVRTextureHeader& sHeader,	CPVRTextureData& sData);// from CPVRTextureHeader/CPVRTextureData pair
	CPVRTexture(
		const unsigned int	u32Width,	// create header info with no data
		const unsigned int	u32Height,
		const unsigned int	u32MipMapCount,
		const unsigned int	u32NumSurfaces,
		const bool			bBorder,
		const bool			bTwiddled,
		const bool			bCubeMap,
		const bool			bVolume,
		const bool			bFalseMips,
		const bool			bAlpha,
		const bool			bFlipped,
		const PixelType		ePixelType,
		const float			fNormalMap);

	CPVRTexture(
		const unsigned int	u32Width,	// create header info for raw data
		const unsigned int	u32Height,
		const unsigned int	u32MipMapCount,
		const unsigned int	u32NumSurfaces,
		const bool			bBorder,
		const bool			bTwiddled,
		const bool			bCubeMap,
		const bool			bVolume,
		const bool			bFalseMips,
		const bool			bAlpha,
		const bool			bFlipped,
		const PixelType		ePixelType,
		const float			fNormalMap,
		uint8				*pPixelData);
	
	// Easy constructor
	CPVRTexture(
		const unsigned int	u32Width,	
		const unsigned int	u32Height,
		const unsigned int	u32MipMapCount=0,
		const unsigned int	u32NumSurfaces=1,
		const PixelType		ePixelType=DX10_R8G8B8A8_UNORM,
		const float			fNormalMap=1.0f,
		const bool			bBorder=false,
		const bool			bTwiddled=false,
		const bool			bCubeMap=false,
		const bool			bVolume=false,
		const bool			bFalseMips=false,
		const bool			bAlpha=false,
		const bool			bFlipped=false,
		uint8				*pPixelData=NULL);
	CPVRTexture(const char* const pszFilename);	// from file depending on extension passed
	CPVRTexture(const uint8* const pPVRData);	// from PVR data with embedded header (contents of PVR file)

	/*******************************************************************************
	* Function Name  : getHeader
	* Description    : returns the CPVRTextureHeader instance from this instance
	*					note this returns a reference
	*******************************************************************************/
	CPVRTextureHeader&	getHeader();
	/*******************************************************************************
	* Function Name  : getData
	* Description    : returns the CPVRTextureData instance from this instance
	*					note this returns a reference
	*******************************************************************************/
	CPVRTextureData&	getData();
	/*******************************************************************************
	*				 : Accessor functions for header values
	* Description    : Correspond to the values held in a pvr texture file.
	*				 : PixelType is the actual texture format
	*				 : Width is the width of the top image of the texture
	*				 : Height is the height of the top image of the texture
	*				 : MipMapCount is the number of MIP-maps present: 0 = top level
	*******************************************************************************/
	void				setData(uint8* pData);
	PixelType			getPixelType() const;
	void				setPixelType(PixelType ePixelType);
	unsigned int		getPrecMode() const;
	unsigned int		getWidth() const;
	unsigned int		getMipWidth(unsigned int iMipMapNum) const;
	void				setWidth(unsigned int u32Width);
	unsigned int		getHeight() const;
	unsigned int		getMipHeight(unsigned int iMipMapNum) const;
	void				setHeight(unsigned int u32Height);
	unsigned int		getMipMapCount() const;
	void				setMipMapCount(unsigned int u32MipMapCount);
	bool				hasMips() const;
	unsigned int		getNumSurfaces() const;
	void				setNumSurfaces(unsigned int u32NumSurfaces);
	size_t				getSurfaceSize() const;
	size_t				getSurfaceSizeInPixels() const;
	size_t				getMIPSize(int mipNumber) const;
	size_t				getMIPSizeInPixels(int mipNumber) const;

	/*******************************************************************************
	*				 : Accessor functions for flag values
	* Description    : Border: a border around the texture
	*					in order to avoid artifacts. See the PVRTC document for more info
	*					Twiddled: Morton order for the texture
	*					CubeMap: does the texture constitute 6 surfaces facing a cube
	*					Volume:	is this a volume texture
	*					NormalMap: a value of 0.0f indicates not a Normal map texture
	*								a non-zero value is taken as the height factor
	*								when calculating the normal vectors
	*					FalseMips: artificially coloured MIP-map levels
	*******************************************************************************/
	bool			isBordered() const;
	void			setBorder(bool bBorder);
	bool			isTwiddled() const;
	void			setTwiddled(bool bTwiddled);
	bool			isCubeMap() const;
	void			setCubeMap(bool bCubeMap);
	bool			isVolume() const;
	void			setVolume(const bool bVolume);
	float			getNormalMap() const;
	void			setNormalMap(const float fNormalMap);
	bool			isNormalMap() const;
	bool			hasFalseMips() const;
	void			setFalseMips(const bool bFalseMips); 
	bool			hasAlpha() const;
	void			setAlpha(const bool bAlpha); 
	bool			isFlipped() const;
	void			setFlipped(const bool bFlipped);

	/*******************************************************************************
	* Function Name  : convertToPrecMode
	* Description    : Converts the data to float/uint16/uint8 from uint8/uint16/float
	*					expanding/contracting the buffer.
	*					If the data is not uint8/uint16/float PVRTHROWs
	*******************************************************************************/
	void				convertToPrecMode(const E_PRECMODE ePrecMode);
	/*******************************************************************************
	* Function Name  : SwapChannels
	* Description    : Swaps the two specified channels.
	*					If the data is not uint8/uint16/uint32/float PVRTHROWs
	*******************************************************************************/
	void SwapChannels(const E_COLOUR_CHANNEL e32ChannelA, const E_COLOUR_CHANNEL e32ChannelB);
	/*******************************************************************************
	* Function Name  : SetChannels
	* Description    : Sets channelTo in this texture to the channelFrom in the new texture.
	*					If the data is not uint8/uint16/uint32/float PVRTHROWs
	*******************************************************************************/
	void setChannel(
		const unsigned int eChannelTo,
		const unsigned int eChannelFrom,
		CPVRTexture& sSourceTexture,
		const unsigned int u32SourceSurfaceNum=0,
		const unsigned int u32DestSurfaceNum=0,
		const int i32MipNum=-1);

	/*******************************************************************************
	* Function Name  : SetChannels
	* Description    : Sets channelTo in this texture to the specified value
	*					If the data is not uint8/uint16/uint32/float PVRTHROWs
	*******************************************************************************/
	void setChannel(
		const unsigned int eChannelTo,
		const unsigned int newValue=255,
		const unsigned int u32DestSurfaceNum=0,
		const int i32MipNum=-1);

	/*******************************************************************************
	* Function Name  : getSurfaceData
	* Description    : Returns a pointer to the surface specified, NULL if it doesn't exist
	*******************************************************************************/
	uint8*				getSurfaceData(unsigned int u32SurfaceNum);

	/*******************************************************************************
	* Function Name  : Append
	* Description    : Appends a texture to the current one if possible. If the current
	*					texture has no surfaces then acts like a copy assignment
	*******************************************************************************/
	void				append(CPVRTexture& sTexture);

	/*******************************************************************************
	* Function Name  : RGBToAlpha
	* Description    : Adds alpha to a texture from the RGB data of the other texture
	*******************************************************************************/
	void				RGBToAlpha(	CPVRTexture& sAlphaTexture,
									const unsigned int u32DestSurfaceNum = 0,
									const unsigned int u32SourceSurfaceNum = 0 );

	/*******************************************************************************
	* Function Name  : writeToFile
	* Description    : writes to a file depending on the extension of the file path passed.
	*******************************************************************************/
	size_t				writeToFile(const char* const strFilename)const ;

	/*******************************************************************************
	* Function Name  : writeIncludeFile32Bits
	* Description    : writes to a C++ compatible header file (u32TextureVersion is currently unimplemented)
	*******************************************************************************/
	size_t				writeIncludeFile32Bits(const char* const pszFilename,
							const char* const pszVarName)const ;

	private:
	CPVRTextureHeader	m_sHeader;
	CPVRTextureData		m_sData;

};

#ifdef __APPLE__
#pragma GCC visibility pop
#endif
	
	
}
#endif // CPVRTEXTURE_H

/*****************************************************************************
 End of file (PVRTexture.h)
*****************************************************************************/


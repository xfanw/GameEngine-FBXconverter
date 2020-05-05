#ifndef IMAGE_H
#define IMAGE_H

#include "Rect.h"
#include "Texture.h"
#include "ImageLink.h"

class Image : public ImageLink
{
public:
	enum Name
	{
		Alien_Blue,
		Alien_Green,
		Alien_Red,
		Stitch,
		FONT,
		Not_Initialized,
	};
public:
	// public methods: -------------------------------------------------------------
	Image();
	Image(Image::Name imageName, Texture::Name textName, Rect rect);
	Image(Image::Name imageName, Texture *pTexture, Rect rect);

	void Set(Image::Name imageName, Texture::Name textName, Rect rect);
	void Set(Image::Name imageName, Texture *pTexture, Rect rect);
	//EnumName getName();

public:
	// data: -----------------------------------------------------------------------
	Image::Name	    imageName;
	Texture			*pText;
	Rect			imageRect;
};




#endif
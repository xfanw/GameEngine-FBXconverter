#include "TextureMan.h"
#include "Image.h"
#include "Rect.h"


// public methods: -------------------------------------------------------------

Image::Image()
{
	this->imageName = Image::Name::Not_Initialized;
	this->imageRect.clear();
	this->pText = 0;
}

Image::Image(Image::Name _imageName, Texture *_pText, Rect _rect)
{
	this->imageName = _imageName;
	this->pText = _pText;
	this->imageRect = _rect;
}

Image::Image(Image::Name _imageName, Texture::Name _textName, Rect _rect)
{
	this->imageName = _imageName;
	this->pText = TextureMan::Find(_textName);
	this->imageRect = _rect;
}

void Image::Set(Image::Name _imageName, Texture *_pText, Rect _rect)
{
	this->imageName = _imageName;
	this->pText = _pText;
	this->imageRect = _rect;
}

void Image::Set(Image::Name _imageName, Texture::Name _textName, Rect _rect)
{
	this->imageName = _imageName;
	this->pText = TextureMan::Find(_textName);
	this->imageRect = _rect;
}

//
//EnumName Image::getName()
//{
//	return ImageName((EnumName::Name)this->imageName);
//}
//----------------------------------------------------------------------------
// Copyright 2020,  all rights reserved.
//----------------------------------------------------------------------------

#ifndef TEXTURE_H
#define TEXTURE_H

#include <sb7.h>
#include "TextureLink.h"

class Texture: public TextureLink
{
public:
	static const unsigned int ASSET_NAME_SIZE = 64;

	enum class Name
	{
		STONES,
		RED_BRICK,
		DUCKWEED,
		ROCKS,
		INVADERS,
		CONSOLAS,
		NOT_INITIALIZED
	};

public:
	Texture();

	void set(const char * const _assetName,
			 Texture::Name _name,
			 GLuint _TextureID,
			 GLenum minFilter,
			 GLenum magFilter,
			 GLenum horizWrapMode,
			 GLenum vertWrapMode,
			 int _width,
			 int _height);
	
	virtual ~Texture() = default;
private:
	char assetName[Texture::ASSET_NAME_SIZE];

public:
	Texture::Name name;
	GLuint textureID;
	GLenum minFilter;
	GLenum magFilter;
	GLenum horizWrapMode;
	GLenum vertWrapMode;
	int width;
	int height;
};

#endif

// --- End of File --------------------------------------------------

//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <sb7.h>
#include <stdlib.h>
#include <assert.h>


#include "TGA.h"
#include "TextureMan.h"

TextureMan::TextureMan()
{
	this->active = nullptr;
}

TextureMan * TextureMan::privGetInstance()
{
	// This is where its actually stored (BSS section)
	static TextureMan textMan;
	return &textMan;
}

void TextureMan::Create()
{
	TextureMan* pTextMan = TextureMan::privGetInstance();
	assert(pTextMan);
}

void TextureMan::Destroy()
{
	TextureMan *pTextMan = TextureMan::privGetInstance();
	assert(pTextMan);

	TextureLink *pLink = pTextMan->active;

	while (pLink != nullptr)
	{
		TextureLink *pTmp = pLink;
		pLink = pLink->next;
		pTextMan->privRemove(pTmp, pTextMan->active);
		delete pTmp;
	}
}

void TextureMan::Add(const char * const _assetName, const Texture::Name _name)
{
	GLuint               textureID;
	GLuint *pTextureID = &textureID;

	// Get the instance to the manager
	TextureMan *pTextMan = TextureMan::privGetInstance();

	// Load the texture and get the textureID
	int width;
	int height;
	pTextMan->privLoadTexture(_assetName, pTextureID, width, height);

	// Create a Texture
	Texture *pNode = new Texture();

	// initialize it
	pNode->set(_assetName, _name, textureID, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,width, height );

	// Now add it to the manager
	pTextMan->privAddToFront(pNode, pTextMan->active);
}

GLuint TextureMan::FindID(const Texture::Name _name)
{
	// Get the instance to the manager
	TextureMan *pTextMan = TextureMan::privGetInstance();

	Texture *pNode = (Texture *)pTextMan->active;
	while (pNode != 0)
	{
		if (pNode->name == _name)
		{
			// found it
			break;
		}

		pNode = (Texture *)pNode->next;
	}

	return pNode->textureID;
}

Texture *TextureMan::Find(const Texture::Name _name)
{
	// Get the instance to the manager
	TextureMan *pTextMan = TextureMan::privGetInstance();

	Texture *pNode = (Texture *)pTextMan->active;
	while (pNode != 0)
	{
		if (pNode->name == _name)
		{
			// found it
			break;
		}

		pNode = (Texture *)pNode->next;
	}

	return pNode;
}




void TextureMan::DeleteAllTextures()
{
	// Get the instance to the manager
	TextureMan *pTextMan = TextureMan::privGetInstance();

	Texture *pNode = (Texture *)pTextMan->active;
	while (pNode != 0)
	{
		// bye bye
		glDeleteTextures(1, &pNode->textureID);

		// next
		pNode = (Texture *)pNode->next;
	}
}

void TextureMan::privAddToFront(TextureLink *node, TextureLink *&head)
{
	assert(node != 0);

	if (head == 0)
	{
		head = node;
		node->next = 0;
		node->prev = 0;
	}
	else
	{
		node->next = head;
		head->prev = node;
		head = node;
	}
}

void TextureMan::privRemove(TextureLink *pNode, TextureLink *&poHead)
{
	assert(pNode);

	if (pNode->prev != nullptr)
	{	// middle or last node
		pNode->prev->next = pNode->next;
	}
	else
	{  // first
		poHead = pNode->next;
	}

	if (pNode->next != nullptr)
	{	// middle node
		pNode->next->prev = pNode->prev;
	}

}

void TextureMan::privLoadTexture(const char * const _assetName, GLuint *&textureID, int &width, int &height)
{
	// Get an ID, for textures (OpenGL poor man's handle)
	glGenTextures(1, textureID);

	// Bind it.
	glBindTexture(GL_TEXTURE_2D, *textureID);

	// Loat the texture
	this->privLoadTGATexture(_assetName, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, width, height);
}

// Load a TGA as a 2D Texture. Completely initialize the state
bool TextureMan::privLoadTGATexture(const char *szFileName, GLint minFilter, GLint magFilter, GLint horizWrapMode, GLint vertWrapMode, int &nWidth, int &nHeight)
{
	GLbyte *pBits;
	int  nComponents;
	GLenum eFormat;

	// Read the texture bits
	pBits = gltReadTGABits(szFileName, &nWidth, &nHeight, &nComponents, &eFormat);
	if (pBits == NULL)
	{
		return false;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, horizWrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vertWrapMode);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBits);

	free(pBits);

	if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
		minFilter == GL_LINEAR_MIPMAP_NEAREST ||
		minFilter == GL_NEAREST_MIPMAP_LINEAR ||
		minFilter == GL_NEAREST_MIPMAP_NEAREST)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	return true;
}

////////////////////////////////////////////////////////////////////
// Allocate memory and load targa bits. Returns pointer to new buffer,
// height, and width of texture, and the OpenGL format of data.
// Call free() on buffer when finished!
// This only works on pretty vanilla targas... 8, 24, or 32 bit color
// only, no palettes, no RLE encoding.
GLbyte *TextureMan::gltReadTGABits(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat)
{
	FILE *pFile;			// File pointer
	TGAHEADER tgaHeader;		// TGA file header
	unsigned long lImageSize;		// Size in bytes of image
	short sDepth;			// Pixel depth;
	GLbyte	*pBits = NULL;          // Pointer to bits

	// Default/Failed values
	*iWidth = 0;
	*iHeight = 0;
	*eFormat = GL_RGB;
	*iComponents = GL_RGB;

	// Attempt to open the file
	fopen_s(&pFile, szFileName, "rb");
	if (pFile == NULL)
		return NULL;

	// Read in header (binary)
	fread(&tgaHeader, 18/* sizeof(TGAHEADER)*/, 1, pFile);


	// Get width, height, and depth of texture
	*iWidth = tgaHeader.width;
	*iHeight = tgaHeader.height;
	sDepth = tgaHeader.bits / 8;

	// Put some validity checks here. Very simply, I only understand
	// or care about 8, 24, or 32 bit targa's.
	if (tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
		return NULL;

	// Calculate size of image buffer
	lImageSize = (unsigned int)tgaHeader.width *  (unsigned int)tgaHeader.height *  (unsigned int)sDepth;

	// Allocate memory and check for success
	pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
	if (pBits == NULL)
		return NULL;

	// Read in the bits
	// Check for read error. This should catch RLE or other 
	// weird formats that I don't want to recognize
	if (fread(pBits, lImageSize, 1, pFile) != 1)
	{
		free(pBits);
		return NULL;
	}

	// Set OpenGL format expected
	switch (sDepth)
	{

	case 3:     // Most likely case
		*eFormat = GL_BGR;
		*iComponents = GL_RGB;
		break;

	case 4:
		*eFormat = GL_BGRA;
		*iComponents = GL_RGBA;
		break;
	case 1:
		assert(false);
		// bad case - keenan
		// i commented out
	  //  *eFormat = GL_LUMINANCE;
	  //  *iComponents = GL_LUMINANCE;
		break;
	default:        // RGB
		// If on the iPhone, TGA's are BGR, and the iPhone does not 
		// support BGR without alpha, but it does support RGB,
		// so a simple swizzle of the red and blue bytes will suffice.
		// For faster iPhone loads however, save your TGA's with an Alpha!

		break;
	}

	// Done with File
	fclose(pFile);

	// Return pointer to image data
	return pBits;
}

// --- End of File --------------------------------------------------


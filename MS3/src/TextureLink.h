//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef TEXTURE_LINK_H
#define TEXTURE_LINK_H

class TextureLink
{
public:
	TextureLink()
	{
		this->next = 0;
		this->prev = 0;
	}

	virtual ~TextureLink()
	{

	}

	TextureLink *next;
	TextureLink *prev;
};

#endif

// --- End of File --------------------------------------------------

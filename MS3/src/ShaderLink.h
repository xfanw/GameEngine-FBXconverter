//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SHADER_LINK_H
#define SHADER_LINK_H

class ShaderLink
{
public:
	ShaderLink()
	{
		this->next = 0;
		this->prev = 0;
	}

	virtual ~ShaderLink()
	{

	}

	ShaderLink *next;
	ShaderLink *prev;
};

#endif

// --- End of File --------------------------------------------------

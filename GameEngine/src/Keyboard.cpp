//----------------------------------------------------------------------------
// Copyright 2020,  all rights reserved.
//----------------------------------------------------------------------------

#include <assert.h>

#include "Game.h"
#include "Keyboard.h"

extern Game *pGame;

// Use this to read keyboard
bool Keyboard::GetKeyState( Keyboard::KEY key )
{
	bool value;

	
	if( pGame->GetKey(key) == GLFW_PRESS )
	{
		value = true;
	}
	else
	{
		value = false;
	}

	return value;
}

// --- End of File --------------------------------------------------

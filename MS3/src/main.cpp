//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Framework.h"
#include "Game.h"

Game *pGame = 0;

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	pGame = new Game("Animation", Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT);
	pGame->Run();
	delete pGame;

	return 0;
}

//--- End of File -------------------------------------------------------------

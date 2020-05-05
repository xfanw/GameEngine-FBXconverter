//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_H
#define GAME_H

#include <sb7.h>

#include "Engine.h"
#include "MathEngine.h"
#include "Time.h"
#include "Timer.h"

class Game : public Engine
{
public:

	const static int SCREEN_WIDTH = 1200;
	const static int SCREEN_HEIGHT = 900;

public:
	// constructor
	Game() = delete;
	Game(const Game &) = delete;
	Game & operator=(const Game &) = delete;
	~Game() = default;

	Game(const char* windowName, const int Width, const int Height);

	virtual void Initialize() override;
	virtual void LoadContent() override;
	unsigned char* readFile(const char* fileName);
	virtual void Update(float currentTime) override;
	virtual void Draw() override;
	virtual void UnLoadContent() override;
	virtual void ClearBufferFunc() override;

	// KILLED - no need
	// virtual void onResize(int w, int h) override;

public:
	// Nice and clean
	Timer	intervalTimer;
	Timer	globalTimer;
};

#endif

//--- End of File ------------------------------------------------------------

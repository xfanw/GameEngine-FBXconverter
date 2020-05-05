//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef INPUT_MAN_H
#define INPUT_MAN_H

#include "Keyboard.h"
#include "Mouse.h"

class InputMan
{
public:
	InputMan(const InputMan &) = delete;
	InputMan & operator = (const InputMan &) = delete;
	~InputMan();

	static void Create();
	static Keyboard *GetKeyboard();
	static Mouse *GetMouse();
	static void Destroy();
private:
	InputMan();

	static InputMan *privInstance();
	static void privCreate();

	// Data
	static InputMan *pInputMan;
	Keyboard		*pKeyboard;
	Mouse			*pMouse;
};

#endif

// --- End of File --------------------------------------------------

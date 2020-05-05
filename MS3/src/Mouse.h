//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MOUSE_H
#define MOUSE_H

// Wrapper to buffer users from the internal glfw driver
// Converted to enumeration for type safety - yeah!

class Mouse
{
public:
	enum Button
	{
		BUTTON_1 = 0,
		BUTTON_2 = 1,
		BUTTON_3 = 2,
		BUTTON_4 = 3,
		BUTTON_5 = 4,
		BUTTON_6 = 5,
		BUTTON_7 = 6,
		BUTTON_8 = 7,
		BUTTON_LEFT = BUTTON_1,
		BUTTON_RIGHT = BUTTON_2,
		BUTTON_MIDDLE = BUTTON_3
	};

public:
	Mouse();

	// Use this to read mouse buttons
	bool GetKeyState(Button button);

	// get cursor
	void GetCursor(float &xPos, float &yPos);

private:

};


#endif

// --- End of File --------------------------------------------------


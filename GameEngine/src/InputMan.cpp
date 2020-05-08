//----------------------------------------------------------------------------
// Copyright 2020,  all rights reserved.
//----------------------------------------------------------------------------

#include <assert.h>
#include "InputMan.h"

InputMan *InputMan::pInputMan = 0;

void InputMan::Create( )
{
	InputMan::privCreate( );
}

InputMan::InputMan( )
{
	this->pKeyboard = new Keyboard(  );
	this->pMouse = new Mouse();
	assert(this->pKeyboard);
}

void InputMan::privCreate(   )
{
	InputMan::pInputMan = new InputMan(  );
	assert(pInputMan);
}

InputMan::~InputMan()
{
	InputMan* pMan = InputMan::privInstance();
	assert(pMan);
	delete pMan->pKeyboard;
    delete pMan->pMouse;
}

Keyboard *InputMan::GetKeyboard()
{
	InputMan *pMan = InputMan::privInstance();
	return pMan->pKeyboard;
}

Mouse *InputMan::GetMouse()
{
	InputMan *pMan = InputMan::privInstance();
	return pMan->pMouse;
}

InputMan *InputMan::privInstance()
{
	assert( pInputMan );
	return pInputMan;
}

void InputMan::Destroy()
{
	InputMan* pMan = InputMan::privInstance();
	assert(pMan);

	delete pMan->pInputMan;

}
// --- End of File --------------------------------------------------


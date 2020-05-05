//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Framework.h"

#include "MathEngine.h"
#include "GameObjectNull.h"
#include "GraphicsObject.h"

#define UNUSED_VAR(x) (void)x 

GameObjectNull::GameObjectNull(GraphicsObject_Null  *pGraphicsObject)
	: GameObject(pGraphicsObject)
{
	assert(pGraphicsObject != 0);

}

GameObjectNull::~GameObjectNull()
{
}


void GameObjectNull::Update(Time currentTime)
{
	UNUSED_VAR(currentTime);

	// Goal: update the world matrix
	Matrix Identity(Matrix::Special::Identity);
	*this->poWorld = Identity;

	// push to graphics object
	this->poGraphicsObject->SetWorld(*this->poWorld);
}



//--- End of File ------------------------------------------------------------

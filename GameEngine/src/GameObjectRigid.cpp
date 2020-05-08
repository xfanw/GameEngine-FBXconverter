//----------------------------------------------------------------------------
// Copyright 2020,  all rights reserved.
//----------------------------------------------------------------------------

#include "MathEngine.h"
#include "GameObjectRigid.h"
#include "GraphicsObject.h"
#include "CameraMan.h"
GameObjectRigid::GameObjectRigid(GraphicsObject  *pGraphicsObject)
	: GameObject(pGraphicsObject)
{
	assert(pGraphicsObject != 0);

	this->poScale = new Vect(1.0f, 1.0f, 1.0f);
	assert(this->poScale);

	this->poPos = new Vect(0.0f, 0.0f, 0.0f);
	assert(this->poPos);

	this->angle = 0.0f;
}

GameObjectRigid::~GameObjectRigid()
{
	delete this->poPos;
	delete this->poScale;
}

void GameObjectRigid::privUpdate(Time )
{

}

void GameObjectRigid::Update(Time currentTime)
{
	// Goal: update the world matrix
	this->privUpdate(currentTime);

	// update the bounding volume based on world matrix
	this->baseUpdateBoundingSphere();

	// push to graphics object
	this->poGraphicsObject->SetWorld(*this->poWorld);
}


//--- End of File ------------------------------------------------------------

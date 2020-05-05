//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Game.h"
#include "MathEngine.h"
#include "GameObjectSimple.h"
#include "GameObject.h"
#include "GraphicsObject.h"
#include "Model.h"


GameObjectSimple::GameObjectSimple(GraphicsObject  *pGraphicsObject)
	: GameObject(pGraphicsObject)
{
	this->poWorld = new Matrix(Matrix::Special::Identity);
	this->poTrans = new Vect(0.0f, 0.0f, 0.0f);
	this->poScale = new Vect(1.0f, 1.0f, 1.0f);
	assert(poWorld);
	assert(poTrans);
	assert(poScale);
	assert(pGraphicsObject);

	this->rotY = 0.0f;
	this->rotZ = 0.0f;
	this->deltaRot = 0.0f;
}

GameObjectSimple::~GameObjectSimple()
{
	delete this->poWorld;
	delete this->poTrans;
	delete this->poScale;
	
}



void GameObjectSimple::privUpdate(Time currentTime)
{
	UNUSED_VAR(currentTime);

	// Goal: update the world matrix

	this->rotY += deltaRot;
	this->rotZ += 0.01f;

	Matrix ScaleA(Matrix::Scale::XYZ, *this->poScale);
	Matrix TransA(Matrix::Trans::XYZ, *this->poTrans);
	Matrix RotY(Matrix::Rot1::Y, this->rotY);
	Matrix RotZ(Matrix::Rot1::Z, this->rotZ);

	// world matrix
	*this->poWorld = ScaleA * RotY * RotZ * TransA;
}



void GameObjectSimple::Update(Time currentTime)
{
	// Goal: update the world matrix
	this->privUpdate(currentTime);

	// push to graphics object
	this->poGraphicsObject->SetWorld(*this->poWorld);
}

void GameObjectSimple::SetWorld(Matrix* _pWorld)
{
	assert(_pWorld);
	*this->poWorld = *_pWorld;
}

void GameObjectSimple::SetPos(const Vect& pos)
{
	*this->poTrans = pos;
}

void GameObjectSimple::SetScale(const Vect& inScale)
{
	// deep copy
	*this->poScale = inScale;
}

void GameObjectSimple::SetDeltaRot(float delta)
{
	this->deltaRot = delta;
}

void GameObjectSimple::Draw()
{
	GraphicsObject* pGraphicsObj = this->GetGraphicsObject();
	assert(pGraphicsObj);

	pGraphicsObj->Render();
}
//--- End of File ------------------------------------------------------------
//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Game.h"
#include "MathEngine.h"
#include "GameObject.h"
#include "GraphicsObject.h"
#include "Model.h"
#include "FrameBucket.h"

// YUCK - REFACTOR
//extern Game *pGame;

Matrix *GameObject::GetWorld()
{
	return this->poWorld;
}


GameObject::GameObject(GraphicsObject  *pGraphicsObject)
	: poGraphicsObject(pGraphicsObject)
{
	this->poWorld = new Matrix(Matrix::Special::Identity);
	assert(poWorld);

	this->mDrawEnable = true;

	assert(pGraphicsObject != 0);
}

bool GameObject::GetDrawEnable()
{
	return this->mDrawEnable;
}

void GameObject::SetDrawEnable(bool val)
{
	this->mDrawEnable = val;
}

GraphicsObject *GameObject::GetGraphicsObject()
{
	return this->poGraphicsObject;
}

GameObject::~GameObject()
{
	delete this->poWorld;
	//delete pSphere;
	delete this->poGraphicsObject;

}

void GameObject::baseUpdateBoundingSphere()
{
	// Get the ref Sphere
	Sphere *pRefSphere = poGraphicsObject->GetModel()->pRefSphere;
	assert(pRefSphere);

	// Get the curr Sphere
	Sphere *pCurrSphere = poGraphicsObject->poCurrSphere;
	assert(pCurrSphere);

	// Update... cntr process through the world
	// Assuming a uniform scaling
	Vect A = pRefSphere->cntr;
	Vect radius(1.0f, 0.0f, 0.0f);
	Vect B = A + radius;
	Vect A_out = A * (*this->poWorld);
	Vect B_out = B * (*this->poWorld);

    pCurrSphere->rad = (B_out[x] - A_out[x])*pRefSphere->rad;
	pCurrSphere->cntr = A_out;
}

void GameObject::SetWorld(Matrix *_pWorld)
{
	assert(_pWorld);
	*this->poWorld = *_pWorld;
}

void GameObject::Draw()
{
	GraphicsObject *pGraphicsObj = this->GetGraphicsObject();
	assert(pGraphicsObj);

	pGraphicsObj->Render();
}

//--- End of File ------------------------------------------------------------
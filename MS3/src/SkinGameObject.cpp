//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MathEngine.h"
#include "SkinGameObject.h"
#include "GraphicsObject.h"
#include "FrameBucket.h"
#include "AnimManager.h"
#include "GraphicsObject_SkinConstColor.h"
#include "GameObjectAnim.h"

SkinGameObject::SkinGameObject(GraphicsObject  *pGraphicsObject)
	: GameObject(pGraphicsObject)
{
	assert(pGraphicsObject != 0);
	
}

SkinGameObject::~SkinGameObject()
{

}

void SkinGameObject::privUpdate(Time )
{
	Anim* pAnim = AnimManager::GetCurrent();
	//pAnim->SetupBoneWorld();
	this->pWorldAll = pAnim->poWorldAll;
}

void SkinGameObject::Update(Time currentTime)
{
	// Goal: update the world matrix
	this->privUpdate(currentTime);


	// push to graphics object
	((GraphicsObject_SkinConstColor*)this->poGraphicsObject)->updateBoneWorld(this->pWorldAll);
}


//--- End of File ------------------------------------------------------------

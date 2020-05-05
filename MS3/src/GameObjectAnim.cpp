//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MathEngine.h"
#include "GameObjectAnim.h"
#include "GraphicsObject.h"
#include "FrameBucket.h"
#include "AnimManager.h"


GameObjectAnim::GameObjectAnim(GraphicsObject  *pGraphicsObject)
	: GameObject(pGraphicsObject)
{
	assert(pGraphicsObject != 0);

	this->poDof = new Vect(0.0f, 0.0f, 0.0f);
	assert(poDof);

	this->poUp = new Vect(0.0f, 1.0f, 0.0f);
	assert(poUp);

	this->poScale = new Vect(1.0f, 1.0f, 1.0f);
	assert(poScale);

	this->poPos = new Vect(0.0f, 0.0f, 0.0f);
	assert(poPos);

	this->indexBoneArray = 0;

	this->poLocal = new Matrix(Matrix::Special::Identity);
	assert(poLocal);

	this->poBoneOrientation = new Matrix(Matrix::Special::Identity);
	assert(poBoneOrientation);
	SetDrawEnable(false);
}

GameObjectAnim::~GameObjectAnim()
{
	delete this->poDof;
	delete this->poUp;
	delete this->poPos;
	delete this->poScale;
	delete this->poLocal;
	delete this->poBoneOrientation;
}

void GameObjectAnim::privUpdate(Time )
{
	//GameObject *pBoneParent = (GameObject *)this->GetParent();
	//assert(pBoneParent != 0);

	//Matrix ParentWorld = *pBoneParent->GetWorld();

	//// REMEMBER this is for Animation and hierachy, you need to handle models differently
	//// Get the result bone array, from there make the matrix
	//Bone *bResult = AnimManager::GetCurrent()->pFrameHead->poBone;
	////Trace::out("Bone[%d]\nT: %f %f %f\n Q: %f %f %f\n S: %f %f %f\n", indexBoneArray,
	////	bResult[indexBoneArray].T.x(), bResult[indexBoneArray].T.y(), bResult[indexBoneArray].T.z(),
	////	bResult[indexBoneArray].Q.qx(), bResult[indexBoneArray].Q.qy(), bResult[indexBoneArray].Q.qz(),
	////	bResult[indexBoneArray].S.x(), bResult[indexBoneArray].S.y(), bResult[indexBoneArray].S.z());
	//Matrix T = Matrix(Matrix::Trans::XYZ, bResult[indexBoneArray].T);
	//Matrix S = Matrix(Matrix::Scale::XYZ, bResult[indexBoneArray].S);
	//Quat   Q = bResult[indexBoneArray].Q;

	//// Isn't it awesome that we can multiply Quat with matrices!
	//Matrix M = S * Q * T;
	//*this->poLocal = M;

	//// Goal: update the world matrix
	//*this->poWorld = *this->poLocal * ParentWorld;
	

}

void GameObjectAnim::Update(Time currentTime)
{
	// Goal: update the world matrix
	//this->privUpdate(currentTime);
	*this->poWorld = AnimManager::GetCurrent()->poWorldAll[indexBoneArray];
	// update the bounding volume based on world matrix
	this->baseUpdateBoundingSphere();

	// push to graphics object
	Matrix mTmp = *this->poBoneOrientation;

	// push to graphics object
	this->poGraphicsObject->SetWorld(mTmp);
}

void GameObjectAnim::SetIndex(int val)
{
	this->indexBoneArray = val;
}

void GameObjectAnim::SetAnimName(const char* name)
{
	strcpy_s(animName,name);
}

const char* GameObjectAnim::GetAnimName()
{
	return animName;
}

void GameObjectAnim::SetBoneOrientation(const Matrix &tmp)
{
	*this->poBoneOrientation = tmp;
}

Matrix GameObjectAnim::GetBoneOrientation(void) const
{
	return Matrix(*this->poBoneOrientation);
}


//--- End of File ------------------------------------------------------------

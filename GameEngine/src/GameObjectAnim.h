//----------------------------------------------------------------------------
// Copyright 2020,  all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_ANIM_H
#define GAME_OBJECT_ANIM_H

#include "GameObject.h"
#include "MathEngine.h"
#include "GraphicsObject.h"

class GameObjectAnim: public GameObject
{
public:
	GameObjectAnim(GraphicsObject  *graphicsObject);

	// Big four
	GameObjectAnim() = delete;
	GameObjectAnim(const GameObjectAnim &) = delete;
	GameObjectAnim &operator=(GameObjectAnim &) = delete;
	virtual ~GameObjectAnim() override;

	virtual void Update(Time currentTime);

	// Accessors
	Matrix GetBoneOrientation(void) const;
	void SetBoneOrientation(const Matrix &);
	void SetIndex(int val);
	void SetAnimName(const char* name);
	const char* GetAnimName();
private:
	void privUpdate(Time currentTime);

public:

	Vect *poScale;
	Vect *poPos;
	Vect *poDof;
	Vect *poUp;

	Matrix      *poLocal;
	Matrix		*poBoneOrientation;
	int         indexBoneArray;
	char		animName[64];


};


#endif

//--- End of File ------------------------------------------------------------

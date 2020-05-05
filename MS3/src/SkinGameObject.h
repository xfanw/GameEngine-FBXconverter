//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_SKIN_H
#define GAME_OBJECT_SKIN_H

#include "GameObject.h"
#include "MathEngine.h"
#include "GraphicsObject.h"

class SkinGameObject: public GameObject
{
public:
	SkinGameObject(GraphicsObject  *graphicsObject);

	// Big four
	SkinGameObject() = delete;
	SkinGameObject(const SkinGameObject &) = delete;
	SkinGameObject &operator=(SkinGameObject &) = delete;
	virtual ~SkinGameObject() override;

	virtual void Update(Time currentTime);

private:
	void privUpdate(Time currentTime);

public:
	Matrix* pWorldAll;
};


#endif

//--- End of File ------------------------------------------------------------

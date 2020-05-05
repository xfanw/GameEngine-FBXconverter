//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef BOUNDING_SPHERE_GAME_OBJECT_H
#define BOUNDING_SPHERE_GAME_OBJECT_H

#include "MathEngine.h"
#include "GameObject.h"

class BoundingSphereGameObject : public GameObject
{
public:
	BoundingSphereGameObject(GraphicsObject  *graphicsObject);

	// Big four
	BoundingSphereGameObject() = delete;
	BoundingSphereGameObject(const BoundingSphereGameObject &) = delete;
	BoundingSphereGameObject &operator=(BoundingSphereGameObject &) = delete;
	virtual ~BoundingSphereGameObject();

	void SetBoundingSphereReference(GameObject *pGameObj);

	virtual void Update(Time currentTime) override;

private:
	void privUpdate(Time currentTime);
	void privUpdateBoundingSphereReference(Time currentTime);

	// Data: ------------------
	GameObject *pRefGameObj;

public:
	Vect *pScale;
	Vect *pTrans;
};


#endif

//--- End of File ------------------------------------------------------------

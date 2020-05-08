//----------------------------------------------------------------------------
// Copyright 2019,  all rights reserved.
//----------------------------------------------------------------------------

#ifndef SIMPLE_GAME_OBJECT_H
#define SIMPLE_GAME_OBJECT_H

#include "GameObject.h"
#include "MathEngine.h"
#include "GraphicsObject.h"


class GameObjectSimple: public GameObject
{

public:
	GameObjectSimple(GraphicsObject  *graphicsObject);

	// Big four
	GameObjectSimple() = delete;
	GameObjectSimple(const GameObjectSimple &) = delete;
	GameObjectSimple &operator=(GameObjectSimple &) = delete;
	virtual ~GameObjectSimple();

	void Draw();
	virtual void Update(Time t) override;

	void SetWorld(Matrix* pWorld);
	void SetPos(const Vect& pos);
	void SetScale(const Vect& scale);
	void SetDeltaRot(float delta);

	

private:
	void privUpdate(Time t);

public:   // add accessors later
	Matrix* poWorld;
	Vect* poTrans;
	Vect* poScale;

	float           deltaRot;
	float			rotY;
	float			rotZ;

};


#endif

//--- End of File ------------------------------------------------------------

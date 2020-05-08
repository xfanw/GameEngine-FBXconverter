//----------------------------------------------------------------------------
// Copyright 2020,  all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "MathEngine.h"
#include "GraphicsObject.h"
#include "PCSNode.h"

class GameObject : public PCSNode
{
public:
	GameObject(GraphicsObject  *graphicsObject);

	// Big four
	GameObject() = delete;
	GameObject(const GameObject &) = delete;
	GameObject &operator=(GameObject &) = delete;
	virtual ~GameObject();

	virtual void Draw();
	virtual void Update(Time currentTime) = 0;

	// Accessors
	GraphicsObject *GetGraphicsObject();
	Matrix *GetWorld();
	void SetWorld(Matrix *_pWorld);

	bool GetDrawEnable();
	void SetDrawEnable(bool);

public:
	Matrix *poWorld;
	void baseUpdateBoundingSphere();

public:
	Sphere *pSphere;
	GraphicsObject  *poGraphicsObject;
	bool mDrawEnable;
	char pad[3];
};


#endif

//--- End of File ------------------------------------------------------------

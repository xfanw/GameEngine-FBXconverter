//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MODEL_H
#define MODEL_H

#include "sb7.h"

#include "MathEngine.h"
#include "BoundingSphere.h"

struct Vert_xyzuvn
{
	float x;
	float y;
	float z;
	float u;
	float v;
	float nx;
	float ny;
	float nz;
};

struct Tri_index
{
	unsigned int v0;
	unsigned int v1;
	unsigned int v2;
};

class ModelLink
{

public:
	ModelLink()
	{
		this->next = nullptr;
		this->prev = nullptr;
	}

	virtual ~ModelLink()
	{

	}

	ModelLink* next;
	ModelLink* prev;
};

class Model : public ModelLink
{
public:
	enum class NAME {
		SPRITE,
		CUBE,
		PYRAMID,
		NULLMODEL,
		SPHERE,
		SKIN,
		SKIN_BEAR,
		UNDIFINED
	};
public:
	Model();
	Model(Model &copyModel) = delete;
	Model & operator = (Model &copyModel) = delete;
	virtual ~Model();

	// Data
	int numVerts;
	int numTris;

	GLuint vao;
	GLuint vbo[2];

	// Reference sphere... before any transformations are applied
	Sphere *pRefSphere;

protected:
	// load VAO
	virtual void privCreateVAO(const char * const pModelFileName) = 0;

public:

	Model::NAME name;
};


#endif

// --- End of File --------------------------------------------------

//----------------------------------------------------------------------------
// Copyright 2020,  all rights reserved.
//----------------------------------------------------------------------------

#include "Model.h"

Model::Model()
	: numVerts(0), 
	numTris(0),
	vao(0), 
	vbo{0,0},
	pRefSphere(new Sphere())
{

}

Model::~Model()
{
	delete this->pRefSphere;
}

// --- End of File --------------------------------------------------

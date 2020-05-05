//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CUBE_MODEL_H
#define CUBE_MODEL_H

#include "Model.h"
#include "BoundingSphere.h"
//struct Vert_xyzuvn
//{
//	float x;
//	float y;
//	float z;
//	float u;
//	float v;
//	float nx;
//	float ny;
//	float nz;
//};
//
//struct Tri_index
//{
//	unsigned int v0;
//	unsigned int v1;
//	unsigned int v2;
//};

class CubeModel : public Model
{
public:
	CubeModel(const char * const pModelFileName);

	CubeModel() = delete;
	CubeModel(const CubeModel &) = delete;
	CubeModel & operator = (CubeModel &) = delete;
	virtual ~CubeModel();

private:
	virtual void privCreateVAO(const char * const pModelFileName) override;

	// Data ----------
private:
	Vert_xyzuvn* poVert;
	Tri_index* poTri;
};

#endif

// --- End of File --------------------------------------------------

//----------------------------------------------------------------------------
// Copyright 2020,  all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_SKIN_OBJECT_CONST_COLOR_H
#define GRAPHICS_SKIN_OBJECT_CONST_COLOR_H

#include "GraphicsObject.h"
#include "MathEngine.h"
#include "SkinModel.h"
class GraphicsObject_SkinConstColor :public GraphicsObject
{
public:
	GraphicsObject_SkinConstColor(Model *_pModel, ShaderObject *_pShaderObj, Vect &LightColor, Vect &LightPos);
	~GraphicsObject_SkinConstColor();
	void LoadPoseInv(const SkinHeader* pSkinHeader);
	// Rendermaterial contract
	virtual void SetState() override;
	virtual void SetDataGPU() override;
	virtual void Draw() override;
	virtual void RestoreState() override;

	void updateBoneWorld(Matrix* pBW);
	// data:  place uniform instancing here
	Vect *poLightColor;
	Vect *poLightPos;

	Matrix* pBoneWorld;
	Matrix* pPoseInv;

	int numPoseInv;

};

#endif

//--- End of File -------------------------------------------------------------

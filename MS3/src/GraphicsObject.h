//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_H
#define GRAPHICS_OBJECT_H

#include "RenderMaterial.h"
#include "PCSNode.h"
class Matrix;
class ShaderObject;
class Model;
class Sphere;

class GraphicsObject: private RenderMaterial , public PCSNode
{
public:
	GraphicsObject(Model *model, ShaderObject *pShaderObj);

	GraphicsObject() = delete;
	virtual ~GraphicsObject();

	void Render();
	void SetWorld( Matrix &_world);

	Model *GetModel() const;
	Matrix &GetWorld() ;

	Sphere			*poCurrSphere;

protected:
	Model			*pModel;
	Matrix			*poWorld;
	ShaderObject	*pShaderObj;
};

#endif

//--- End of File -------------------------------------------------------------


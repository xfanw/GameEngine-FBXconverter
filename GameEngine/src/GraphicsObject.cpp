//----------------------------------------------------------------------------
// Copyright 2020,  all rights reserved.
//----------------------------------------------------------------------------

#include <assert.h>

#include "GraphicsObject.h"
#include "MathEngine.h"
#include "Model.h"

GraphicsObject::GraphicsObject(Model *_pModel, ShaderObject *_pShaderObj)
	: pModel(_pModel), pShaderObj(_pShaderObj)
{
	assert(_pModel);
	assert(_pShaderObj);
	this->poWorld = new Matrix(Matrix::Special::Identity);
	this->poCurrSphere = new Sphere();
}

GraphicsObject::~GraphicsObject()
{
	delete this->poWorld;
	delete this->poCurrSphere;
}

void GraphicsObject::Render(   )
{
	this->SetState();
	this->SetDataGPU();
	this->Draw();
	this->RestoreState();
}

Model *GraphicsObject::GetModel() const
{
	return this->pModel;
}

Matrix &GraphicsObject::GetWorld() 
{
	return *this->poWorld;
}

void GraphicsObject::SetWorld( Matrix &_world)
{
	*this->poWorld = _world;
}


//--- End of File -------------------------------------------------------------

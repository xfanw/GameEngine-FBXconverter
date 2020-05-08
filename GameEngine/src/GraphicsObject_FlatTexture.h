//----------------------------------------------------------------------------
// Copyright 2020,  all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_FLAT_TEXTURE_H
#define GRAPHICS_OBJECT_FLAT_TEXTURE_H

#include "GraphicsObject.h"
#include "Texture.h"

class GraphicsObject_FlatTexture :public GraphicsObject
{
public:
	GraphicsObject_FlatTexture(Model *_pModel, ShaderObject *_pShaderObj, Texture::Name _name);

	// Rendermaterial contract
	virtual void SetState() override;
	virtual void SetDataGPU() override;
	virtual void Draw() override;
	virtual void RestoreState() override;

	// data:  place uniform instancing here

	Texture::Name textName;

};
#endif

//--- End of File -------------------------------------------------------------

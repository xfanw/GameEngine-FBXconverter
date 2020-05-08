//----------------------------------------------------------------------------
// Copyright 2020,  all rights reserved.
//----------------------------------------------------------------------------

#include <assert.h>

#include "GraphicsObject_FlatTexture.h"
#include "ShaderObject.h"
#include "TextureMan.h"
#include "Model.h"
#include "CameraMan.h"

GraphicsObject_FlatTexture::GraphicsObject_FlatTexture(Model* _pModel, ShaderObject* _pShaderObj, Texture::Name _name)
	: GraphicsObject(_pModel, _pShaderObj), textName(_name)
{
	assert(pModel);
	assert(pShaderObj);
	assert(poWorld);
}

void GraphicsObject_FlatTexture::SetState()
{
	glActiveTexture(GL_TEXTURE0);

	// Bind the texture
	GLuint textureID = TextureMan::FindID(this->textName);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
}

void GraphicsObject_FlatTexture::SetDataGPU()
{
	// use this shader
	this->pShaderObj->SetActive();

	// set the vao
	glBindVertexArray(this->GetModel()->vao);

	Camera* pCam = CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D);

	Matrix world = this->GetWorld();
	Matrix view = pCam->getViewMatrix();
	Matrix proj = pCam->getProjMatrix();

	glUniformMatrix4fv(this->pShaderObj->GetLocation("proj_matrix"), 1, GL_FALSE, (float*)& proj);
	glUniformMatrix4fv(this->pShaderObj->GetLocation("view_matrix"), 1, GL_FALSE, (float*)& view);
	glUniformMatrix4fv(this->pShaderObj->GetLocation("world_matrix"), 1, GL_FALSE, (float*)& world);;
}

void GraphicsObject_FlatTexture::Draw()
{
	// draw
	//glDrawArrays(GL_TRIANGLES, 0, (3 * this->GetModel()->numTris));

	//The starting point of the IBO
	glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
}

void GraphicsObject_FlatTexture::RestoreState()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
}

//--- End of File -------------------------------------------------------------


//----------------------------------------------------------------------------
// Copyright 2020,  all rights reserved.
//----------------------------------------------------------------------------

#include <assert.h>
#include "MathEngine.h"
#include "Model.h"
#include "GraphicsObject_ConstColor.h"
#include "ShaderObject.h"
#include "MathEngine.h"
#include "CameraMan.h"

GraphicsObject_ConstColor::GraphicsObject_ConstColor(Model *_pModel, ShaderObject *_pShaderObj,Vect &LightColor, Vect &LightPos)
	: GraphicsObject(_pModel, _pShaderObj)
{
	this->poLightColor = new Vect(LightColor);
	this->poLightPos = new Vect(LightPos);
}

GraphicsObject_ConstColor::~GraphicsObject_ConstColor()
{
	delete this->poLightColor;
	delete this->poLightPos;
}

void GraphicsObject_ConstColor::SetState()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
}

void GraphicsObject_ConstColor::SetDataGPU()
{
	// set the vao
	glBindVertexArray(this->GetModel()->vao);

	// use this shader
	this->pShaderObj->SetActive();

	Camera *pCam = CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D);

	Matrix world = this->GetWorld();
	Matrix view = pCam->getViewMatrix();
	Matrix proj = pCam->getProjMatrix();

	glUniformMatrix4fv(this->pShaderObj->GetLocation("proj_matrix"), 1, GL_FALSE, (float *)&proj);
	glUniformMatrix4fv(this->pShaderObj->GetLocation("view_matrix"), 1, GL_FALSE, (float *)&view);
	glUniformMatrix4fv(this->pShaderObj->GetLocation("world_matrix"), 1, GL_FALSE, (float *)&world);

	Vect light = *this->poLightPos;
	Vect color = *this->poLightColor;
	glUniform3fv(this->pShaderObj->GetLocation("vLightPos"), 1, (float *)&light);
	glUniform4fv(this->pShaderObj->GetLocation("vColor"), 1, (float *)&color);

}

void GraphicsObject_ConstColor::Draw()
{
	// draw
//	glDrawArrays(GL_TRIANGLES, 0, (3 * this->GetModel()->numVerts));
	//The starting point of the IBO
	glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);

}

void GraphicsObject_ConstColor::RestoreState()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
}

//--- End of File -------------------------------------------------------------

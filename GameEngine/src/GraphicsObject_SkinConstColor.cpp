//----------------------------------------------------------------------------
// Copyright 2020,  all rights reserved.
//----------------------------------------------------------------------------

#include <assert.h>

#include "Model.h"
#include "GraphicsObject_SkinConstColor.h"
#include "ShaderObject.h"
#include "MathEngine.h"
#include "CameraMan.h"

GraphicsObject_SkinConstColor::GraphicsObject_SkinConstColor(Model *_pModel, ShaderObject *_pShaderObj,Vect &LightColor, Vect &LightPos)
	: GraphicsObject(_pModel, _pShaderObj)
{
	this->poLightColor = new Vect(LightColor);
	this->poLightPos = new Vect(LightPos);
}

GraphicsObject_SkinConstColor::~GraphicsObject_SkinConstColor()
{
	delete this->poLightColor;
	delete this->poLightPos;
}

void GraphicsObject_SkinConstColor::LoadPoseInv(const SkinHeader* pSkinHeader)
{
	this->numPoseInv = pSkinHeader->numPoseInv;
	//Trace::out("Tris: %d\n", numPoseInv);

	this->pPoseInv = (Matrix*)((unsigned char*)pSkinHeader + pSkinHeader->poseInvOffset);

	//Frank Verify

	//Matrix* tmp = pPoseInv;
	//for (int i = 0; i < numPoseInv; i++) {
	//	Trace::out("invPose[%d]:\n %f, %f, %f, %f\n %f, %f, %f, %f\n %f, %f, %f, %f\n %f, %f, %f, %f\n", i,
	//		tmp->m0(), tmp->m1(), tmp->m2(), tmp->m3(), tmp->m4(), tmp->m5(), tmp->m6(), tmp->m7(),
	//		tmp->m8(), tmp->m9(), tmp->m10(), tmp->m11(), tmp->m12(), tmp->m13(), tmp->m14(), tmp->m15());
	//	tmp++;
	//}
}

void GraphicsObject_SkinConstColor::SetState()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
}

void GraphicsObject_SkinConstColor::SetDataGPU()
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

	// Skin
	// Add BoneWold
	glUniformMatrix4fv(this->pShaderObj->GetLocation("invM"), 80, GL_FALSE,(float*) pPoseInv);
	glUniformMatrix4fv(this->pShaderObj->GetLocation("boneWorld"), 80, GL_FALSE,(float*) pBoneWorld);

}

void GraphicsObject_SkinConstColor::Draw()
{
	// draw
//	glDrawArrays(GL_TRIANGLES, 0, (3 * this->GetModel()->numVerts));
	//The starting point of the IBO
	glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);

}

void GraphicsObject_SkinConstColor::RestoreState()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
}

void GraphicsObject_SkinConstColor::updateBoneWorld(Matrix* pBW)
{ 
	this->pBoneWorld = pBW;
}

//--- End of File -------------------------------------------------------------

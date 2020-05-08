#include <assert.h>

#include "GraphicsObject_Sprite.h"
#include "TextureMan.h"
#include "Model.h"
#include "Camera.h"
#include "CameraMan.h"
#include "Image.h"

GraphicsObject_Sprite::GraphicsObject_Sprite(Model *model, ShaderObject *pShaderObj, Image *_pImage, Rect rect)
: GraphicsObject(model, pShaderObj)
{	
	assert(_pImage);
	this->pImage = _pImage;

	this->origWidth = rect.width;
	this->origHeight = rect.height;

	this->origPosX = rect.x;
	this->origPosY = rect.y;
	this->poMatrix_uv = new Matrix();
	this->poMatrix_orig = new Matrix();
	UpdateData();
}

GraphicsObject_Sprite::~GraphicsObject_Sprite()
{
	delete this->poMatrix_uv;
	delete this->poMatrix_orig;
}

void GraphicsObject_Sprite::SetState()
{	
	glActiveTexture(GL_TEXTURE0);

	// Bind the texture
	GLuint textureID = this->pImage->pText->textureID;
	glBindTexture(GL_TEXTURE_2D, textureID);

	// comment out one
    glEnable(GL_CULL_FACE);    
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CW);

	// Blends for sprites
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
}

void GraphicsObject_Sprite::SetDataGPU()
{	     
	// Get camera
	Camera *pCam = CameraMan::GetCurrent(Camera::Type::ORTHOGRAPHIC_2D);

	// set the vao
	glBindVertexArray(this->GetModel()->vao);

	// use this shader
	this->pShaderObj->SetActive();

	// World matrix ----------------------------
	Matrix TransToOriginLowerLeft(Matrix::Trans::XYZ, -pCam->getScreenWidth() / 2.0f, -pCam->getScreenHeight() / 2.0f, 0.0f);
	Matrix tmpMatrix = *this->poMatrix_orig * this->GetWorld() * TransToOriginLowerLeft;

	glUniformMatrix4fv(this->pShaderObj->GetLocation("proj_matrix"), 1, GL_FALSE, (float *)&pCam->getProjMatrix());
	glUniformMatrix4fv(this->pShaderObj->GetLocation("view_matrix"), 1, GL_FALSE, (float *)&pCam->getViewMatrix());
	glUniformMatrix4fv(this->pShaderObj->GetLocation("world_matrix"), 1, GL_FALSE, (float *)&tmpMatrix);
	glUniformMatrix4fv(this->pShaderObj->GetLocation("uv_matrix"), 1, GL_FALSE, (float *)this->poMatrix_uv);

} 

void GraphicsObject_Sprite::Draw()
{	     
	// draw
	glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);   //The starting point of the IBO
}

void GraphicsObject_Sprite::RestoreState()
{	     
	glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
}

void GraphicsObject_Sprite::UpdateData()
{
	// Create the UV matrix
	

	float w = pImage->imageRect.width / pImage->pText->width;
	float h = pImage->imageRect.height / pImage->pText->height;
	float u = pImage->imageRect.x / pImage->pText->width;
	float v = pImage->imageRect.y / pImage->pText->height;
	Matrix ScaleUV(Matrix::Scale::XYZ, w, h, 1.0f);
	Matrix TransUV(Matrix::Trans::XYZ, u, v, 0.0f);
	*this->poMatrix_uv = ScaleUV * TransUV;

	// Create the orig Matrix 
	// The initial sprite setup, after this point standard world will adjust
	

	Matrix Scale(Matrix::Scale::XYZ, this->origWidth, this->origHeight, 1.0f);
	*this->poMatrix_orig = Scale;
}

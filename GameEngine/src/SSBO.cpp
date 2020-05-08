//----------------------------------------------------------------------------
// Copyright 2019,  all rights reserved.
//----------------------------------------------------------------------------

#include <sb7.h>

#include "SSBO.h"



SSBO::SSBO() 
	: elementCount(0), 
	elementSize(0)
{
	glGenBuffers(1, &ssbo);
}


SSBO::~SSBO()
{
	this->pBuffer = nullptr;
}

void SSBO::Set(unsigned _elementCount, unsigned _size, void* _inData)
{
	this->elementCount = _elementCount;
	this->elementSize = _size;
	this->pBuffer = _inData;

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr)(elementCount * this->elementSize), this->pBuffer, GL_STATIC_DRAW);
}

unsigned int SSBO::GetLocation() const
{
	return this->ssbo;
}

void SSBO::Bind(unsigned int bindLocation)
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindLocation, this->ssbo);
}

void *SSBO::Map(SSBO::Access accessType)
{
	void *result = glMapNamedBuffer(this->ssbo, GLenum(accessType));
	assert(result);

	return result;
}

void SSBO::Unmap()
{
	glUnmapNamedBuffer(this->ssbo);
}

//--- End of File -------------------------------------------------------------

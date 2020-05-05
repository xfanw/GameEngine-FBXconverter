//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SHADER_OBJECT_H
#define SHADER_OBJECT_H

#include <sb7.h>

#include "ShaderLink.h"

class ShaderObject : public ShaderLink
{
public:
	enum class NAME
	{
		NULL_SHADER,
		COLOR_LIGHT,
		COLOR_SINGLE,
		SPRITE,
		TEXTURE_SIMPLE,
		TEXTURE_POINT_LIGHT,
		SKIN_COLOR_LIGHT,
		COMPUTE,
		COMPUTE_MIXER,
		COMPUTE_TQR,
		COMPUTE_WORLD,
		CONST_COLOR
	};
	enum class TYPE
	{
		VERTEX,
		COMPUTE,
	};

public:
	ShaderObject(ShaderObject::NAME shaderName, const char * const pShaderBaseFileName, ShaderObject::TYPE type);

	ShaderObject() = delete;
	ShaderObject(const ShaderObject &) = delete;
	ShaderObject &operator = (ShaderObject &) = delete;
	virtual ~ShaderObject() = default;

	void SetActive();
	GLint GetLocation(const char * pUniformName);
	// For Compute Shader
	void Dispatch(unsigned int groupsize_x, unsigned int groupsize_y, unsigned int groupsize_z);
private:
	bool privCreateShader(GLuint &programObject, const char * const pShaderBaseFileName, ShaderObject::TYPE type);
	bool privLoadNCompile(GLuint &shaderObject, const char * const pShaderFileName, GLenum shader_type);

public:
	NAME	name;
	TYPE	type;
	GLuint	programObject;

};


#endif

// --- End of File --------------------------------------------------

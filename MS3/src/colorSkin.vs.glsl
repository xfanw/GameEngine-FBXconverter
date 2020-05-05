#version 430 core

layout (location = 0) in vec4 vert;  // x y z  (w=1)
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 norms;

layout (location = 3) in vec4 boneWeights;
layout (location = 4) in ivec4 boneIndicies;


// Uniforms:
uniform mat4 proj_matrix;
uniform mat4 view_matrix;
uniform mat4 invM[80];
uniform mat4 boneWorld[80];

void main(void)
{              
	// Remember: We use ROW major, but OpenGL GLSL multiplies in reverse order!

	 mat4 skin_matrix = boneWorld[boneIndicies.x] * invM[boneIndicies.x] * boneWeights.x +
					    boneWorld[boneIndicies.y] * invM[boneIndicies.y] * boneWeights.y +
					    boneWorld[boneIndicies.z] * invM[boneIndicies.z] * boneWeights.z +
					    boneWorld[boneIndicies.w] * invM[boneIndicies.w] * boneWeights.w;
	
    gl_Position = proj_matrix * view_matrix * skin_matrix * vert;      

}
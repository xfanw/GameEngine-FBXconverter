//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

// COLOR RENDER - Vertex Shader

#version 400 core

// Uniforms:
uniform mat4 world_matrix;
uniform mat4 view_matrix;
uniform mat4 proj_matrix;

// Attributes
in vec4 position;

// Varying
out VS_OUT
{
    vec4 color;
} vs_out;

void main(void)
{              
	// Remember: We use ROW major, but OpenGL GLSL multiplies in reverse order!
    gl_Position = proj_matrix * view_matrix * world_matrix * position;    

	// Position is the color + Grey
    vs_out.color = position * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);
}

// --- End of File -----------------------------------------------------------

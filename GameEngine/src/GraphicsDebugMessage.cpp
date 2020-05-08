#include "sb7.h"
#include "Framework.h"

void MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	UNUSED_VAR(userParam);
	UNUSED_VAR(length);
	UNUSED_VAR(id);
	UNUSED_VAR(source);
	Trace::out("GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			   (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			   type,
			   severity,
			   message);
}

void GraphicsEnableDebug()
{
	// During init, enable debug output
	glEnable(GL_DEBUG_OUTPUT);

#pragma warning( push )
#pragma warning( disable : 4191 )
	// Compiler doesn't like the blind cast...

	glDebugMessageCallback((GLDEBUGPROC)MessageCallback, 0);

#pragma warning( pop )
}

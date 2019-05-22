#include "eglpch.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		EGL_CORE_INFO("[OpenGL Error] ({0}): {1} {2}: {3} \n", error, function, file, line);
		return false;
	}
	return true;
}
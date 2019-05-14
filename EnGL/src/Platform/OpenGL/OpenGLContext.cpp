#include "EnGL/eglpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>

namespace EnGL {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		EGL_CORE_ASSERT(windowHandle, "windowHandle is null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EGL_CORE_ASSERT(status, "Failed to initialize GLAD");

		EGL_CORE_INFO("OpenGL Information:");
		EGL_CORE_INFO("   Vendor: {0}", glGetString(GL_VENDOR));
		EGL_CORE_INFO("   Renderer: {0}", glGetString(GL_RENDERER));
		EGL_CORE_INFO("   Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}
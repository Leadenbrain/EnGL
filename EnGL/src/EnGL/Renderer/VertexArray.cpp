#include "EnGL/eglpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace EnGL {

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		EGL_CORE_ASSERT(false, "RendererAPI not supported");
			case RendererAPI::API::OpenGL:		return new OpenGLVertexArray();
		}

		EGL_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}
}
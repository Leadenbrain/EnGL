#include "EnGL/eglpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace EnGL {

	Texture* Texture::Create(std::string m_TexPath) {

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:			EGL_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
			case RendererAPI::API::OpenGL:		return new OpenGLTexture(m_TexPath);
		}

		EGL_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;

	}
}
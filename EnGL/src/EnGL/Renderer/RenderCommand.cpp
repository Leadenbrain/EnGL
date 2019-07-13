#include "EnGL/eglpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace EnGL {

	RendererAPI * RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
} 
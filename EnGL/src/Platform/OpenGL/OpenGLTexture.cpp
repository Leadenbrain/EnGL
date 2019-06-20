#include "EnGL/eglpch.h"
#include "OpenGLTexture.h"

#include "stb_image/stb_image.h"

namespace EnGL {

	OpenGLTexture::OpenGLTexture(std::string m_TexPath)
		: m_LocalBuffer(nullptr)
	{
		stbi_set_flip_vertically_on_load(1);
		m_LocalBuffer = stbi_load(m_TexPath.c_str(), &m_Width, &m_Height, &m_BPP, 4);
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		if (m_LocalBuffer)
			stbi_image_free(m_LocalBuffer);
	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture::Bind(unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	void OpenGLTexture::Unbind() const 
	{
		glBindTexture(GL_TEXTURE_2D,0);
	}
}



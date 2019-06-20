#pragma once

#include "EnGL/Renderer/Texture.h"

namespace EnGL {

	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(std::string m_TexPath);
		~OpenGLTexture();

		virtual void Bind(unsigned int slot = 0) const override;
		virtual void Unbind() const override;

		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }

	private:
		uint32_t m_RendererID;
		int m_Width, m_Height, m_BPP;
		std::string m_TexPath;
		unsigned char* m_LocalBuffer;
	};
}
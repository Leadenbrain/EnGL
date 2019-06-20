#pragma once

namespace EnGL {

	class EGL_API Texture
	{
	public:
		~Texture() {};

		virtual void Bind(unsigned int slot = 0) const = 0;
		virtual void Unbind() const = 0;

		static Texture* Create(std::string m_FilePath);
	};

}
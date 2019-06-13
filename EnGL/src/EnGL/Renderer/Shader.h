#pragma once

#include <string>

namespace EnGL {
	
	struct ShaderProgramSource
	{
		std::string vertexSrc;
		std::string fragmentSrc;
	};

	class EGL_API Shader
	{
	public:
			Shader(const std::string& filepath);
			~Shader();

			void Bind() const;
			void Unbind() const;
			void SetUniform1f (const std::string& name, float value);
			void SetUniform2f(std::string& name, float v0, float v1);
			void SetUniformMat4f(std::string& name, const glm::mat4 matrix);

	private:
			uint32_t m_RendererID;
			std::string m_Filepath;
			std::unordered_map<std::string, int> m_UniformLocationCache;

			ShaderProgramSource ParseShader(const std::string& filepath);
			unsigned int CompileShader(unsigned int type, const std::string& source);
			unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
			int GetUniformLocation(const std::string& name);
	};

}
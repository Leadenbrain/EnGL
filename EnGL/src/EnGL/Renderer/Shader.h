#pragma once

#include <string>

namespace EnGL {
	
	struct ShaderProgramSource
	{
		std::string vertexSrc;
		std::string fragmentSrc;
	};

	class Shader
	{
		
	public:
			//Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
			Shader(const std::string& filepath);
			~Shader();

			void Bind() const;
			void Unbind() const;
			void SetUniformMat4f(std::string& name, const glm::mat4 proj);

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
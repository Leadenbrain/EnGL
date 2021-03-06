#include "EnGL/eglpch.h"
#include <FreeImage/FreeImage.h>
#include "Shader.h"

#include <glad/glad.h>

namespace EnGL {

	Shader::Shader(const std::string& filepath)
		: m_Filepath(filepath), m_RendererID(0)
	{
		ShaderProgramSource source = ParseShader(filepath);
		m_RendererID = CreateShader(source.vertexSrc, source.fragmentSrc);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	ShaderProgramSource Shader::ParseShader(const std::string& filepath)
	{
		std::ifstream stream(filepath);

		enum class ShaderType
		{
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};

		std::string line;
		std::stringstream ss[2];
		ShaderType type = ShaderType::NONE;
		while(getline(stream, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
					type = ShaderType::VERTEX;
				else if (line.find("fragment") != std::string::npos)
					type = ShaderType::FRAGMENT;
			}
			else
			{
				ss[(int)type] << line << "\n";
			}
		}

		return {ss[0].str(), ss[1].str()};
	}

	unsigned int Shader::CompileShader(const unsigned int type,const std::string& source)
	{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);
			std::cout << "Failed to Compile " << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << "Shader" << std::endl;
			std::cout << message << std::endl;
			glDeleteShader(id);
			return 0;

		}

		EGL_CORE_INFO("Compiled {0} Shader", (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment"));

		return id;
	}


	unsigned int Shader::CreateShader(const std::string& vertexShader,const std::string& fragmentShader) 
	{
		unsigned int program = glCreateProgram();

		unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);


		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}


	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetUniform1f(const std::string& name, float value)
	{
		glUniform1f(GetUniformLocation(name),value);
	}

	void Shader::SetUniform2f(std::string& name, float v0, float v1)
	{
		glUniform2f(GetUniformLocation(name), v0, v1);
	}


	void Shader::SetUniformMat4f(std::string& name, const glm::mat4 matrix)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
	}

	int Shader::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		int location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
			EGL_CORE_WARN("Warning: Uniform '{0}' doesn't exist!", name);
		m_UniformLocationCache[name] = location;
		return location;
	}

	void Shader::ScreenDump(const char* filename, short W, short H) {
	FILE* out = fopen("output.tga","wb");

	char* pixel_data = new char[3*W*H];
	short TGAhead[] = { 0, 2, 0, 0, 0, 0, W, H, 24 };

	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, W, H, GL_BGR, GL_UNSIGNED_BYTE, pixel_data);

	fwrite(&TGAhead, sizeof(TGAhead), 1, out);
	fwrite(pixel_data, 3*W*H, 1, out);
	fclose(out);


	FIBITMAP* bitmap = FreeImage_Load(FIF_TARGA, "output.tga", TARGA_DEFAULT); 
	delete[] pixel_data; 

	FreeImage_Save(FIF_JPEG, bitmap, filename, 0);
}

}
#include "IndexBuffer.h"
#include "eglpch.h"

//#include "Core.h" 	// Need for assert?
namespace EnGL {

	IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
		: m_Count(count)
	{
		EGL_CORE_ASSERT(sizeof(unsigned int) == sizeof(GLuint));

		GLCall(glGenBuffers(1, &m_IndexBuffer));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
	}

	IndexBuffer::~IndexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_IndexBuffer));
	}

	IndexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer));
	}

	IndexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
}
#include "VertexBuffer.h"
#include "eglpch.h"

//#include "Core.h" 	// Need for assert?
namespace EnGL {

	VertexBuffer::VertexBuffer(const unsigned int* data, unsigned int count)
		: m_Count(count)	//Instead of count, just feed in size? Think this is cleaner front-end
	{
		EGL_CORE_ASSERT(sizeof(unsigned int) == sizeof(GLuint));

		GLCall(glGenBuffers(1, &m_VertexBuffer));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer));
		GLCall(glBufferData(GL_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
	}

	VertexBuffer::~VertexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_VertexBuffer));
	}

	VertexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer));
	}

	VertexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}
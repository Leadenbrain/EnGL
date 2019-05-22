#pragma once

#include "VertexBuffer.h"

namespace EnGL {
	
	class VertexBufferLayout;

	class VertexArray
	{
	private:
		unsigned int m_VertexArray;
		unsigned int m_Count;
	public:
		VertexArray(const unsigned int* data, unsigned int count);
		~VertexArray();

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

		void Bind() const;
		void UnBind() const;
	};

}
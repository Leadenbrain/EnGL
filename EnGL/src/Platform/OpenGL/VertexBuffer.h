#pragma once

namespace EnGL {
	
	class VertexBuffer
	{
	private:
		unsigned int m_VertexBuffer;
		unsigned int m_Count;
	public:
		VertexBuffer(const unsigned int* data, unsigned int count);
		~VertexBuffer();

		void Bind() const;
		void UnBind() const;

		inline unsigned int GetCount() const {return m_Count;}
	};

}
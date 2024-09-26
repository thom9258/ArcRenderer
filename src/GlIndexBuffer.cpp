#include "../arc/GlIndexBuffer.hpp"

namespace arc {
	

index_buffer::index_buffer(const vector_type& indices)
	: m_length(indices.size())
{
	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	const auto buffersize = m_length * sizeof(index_type);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffersize, indices.data(), GL_STATIC_DRAW);
}
	
GLuint index_buffer::get_buffer()
{
	return m_ebo;
}
	
GLuint index_buffer::length()
{
	return m_length;
}

}

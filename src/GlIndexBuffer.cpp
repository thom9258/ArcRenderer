#include "../arc/GlIndexBuffer.hpp"

namespace arc {
	

index_buffer::index_buffer(const vector_type& indices)
{
	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	const auto buffersize = indices.size() * sizeof(index_type);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffersize, indices.data(), GL_STATIC_DRAW);
}
	
GLuint index_buffer::get_buffer()
{
	return m_ebo;
}

}

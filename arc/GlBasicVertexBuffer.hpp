#pragma once

#include "GlContext.hpp"

#include <vector>
#include <functional>
#include <optional>

namespace arc {

template <typename Vertex>
class basic_vertex_buffer
{
public:
	using vertex_type = Vertex;
	using vector_type = std::vector<vertex_type>;
	
	explicit basic_vertex_buffer(const vector_type& vertices);

	[[nodiscard]]
	GLuint get_buffer();

	static void bind_layout();

	[[nodiscard]]
	static GLuint get_layout();

	[[nodiscard]]
	GLuint length();

private:
	GLuint m_vbo;
	GLuint m_length;
	static std::optional<GLuint> g_vao_layout;
};
	

/** **********************************************************************
 * Template Definitions
 */ 

// Definiton of the templated static variable
template <typename Vertex>
std::optional<GLuint> basic_vertex_buffer<Vertex>::g_vao_layout{std::nullopt};
	
template <typename Vertex>
basic_vertex_buffer<Vertex>::basic_vertex_buffer(const vector_type& vertices)
	: m_length(vertices.size())
{
	glGenBuffers(1, &m_vbo);  
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);  
	const auto buffersize = vertices.size() * sizeof(vertex_type);
	glBufferData(GL_ARRAY_BUFFER, buffersize, vertices.data(), GL_STATIC_DRAW);
}

template <typename Vertex>
GLuint basic_vertex_buffer<Vertex>::get_buffer()
{
	return m_vbo;
}

template <typename Vertex>
void basic_vertex_buffer<Vertex>::bind_layout()
{
	glBindVertexArray(get_layout());
}

template <typename Vertex>
GLuint basic_vertex_buffer<Vertex>::get_layout()
{
	if (!basic_vertex_buffer<Vertex>::g_vao_layout.has_value())
		basic_vertex_buffer<Vertex>::g_vao_layout = std::invoke(Vertex::create_layout);
	return basic_vertex_buffer<Vertex>::g_vao_layout.value();
}
	
template <typename Vertex>
GLuint basic_vertex_buffer<Vertex>::length()
{
	return m_length;
}

}

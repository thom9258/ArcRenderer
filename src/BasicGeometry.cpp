#include "../arc/GlContext.hpp"

namespace arc {
	


std::pair<VertexBuffer_PosTex::vector_type, arc::IndexBuffer::vector_type> 
create_unit_plane()
{
    const VertexBuffer_PosTex::vector_type vertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f},  {0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f},   {0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f},  {1.0f, 1.0f}}
    };
    const IndexBuffer::vector_type indices = {
        0, 1, 2, 2, 3, 0
    };
    return {vertices, indices};
}
 

GLuint Vertex_PosTex::get_layout()
{
	if (!m_layout)
		m_layout = create_layout();
	return m_layout;
}


}

#pragma once

#include "GlContext.hpp"

namespace arc {
	
struct Vertex_PosTex {
    glm::vec3 pos{};
    glm::vec2 uv{};
	
    [[nodiscard]]
    static GLuint get_layout();

private:
    [[nodiscard]]
    static GLuint create_layout();
	static std::optional<GLuint> m_layout;
};
	




std::pair<VertexBuffer_PosTex::vector_type, arc::IndexBuffer::vector_type> 
create_unit_plane();

}

#pragma once

#include "GlContext.hpp"

#include <vector>
#include <functional>
#include <optional>

namespace arc {

class index_buffer
{
public:
	using index_type = uint32_t;
	using vector_type = std::vector<index_type>;
	
	index_buffer(const vector_type& indices);

	[[nodiscard]]
	GLuint get_buffer();

	[[nodiscard]]
	GLuint length();

private:
	GLuint m_ebo;
	GLuint m_length;
};

}	

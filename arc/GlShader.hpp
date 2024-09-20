#pragma once

#include "GlContext.hpp"

#include <optional>
#include <string>

namespace arc {


class shader {
public:
	using uniform = std::optional<GLuint>;
	
	[[nodiscard]]
    static std::unique_ptr<shader> create(const char* vertex_source,
										  const char* fragment_source);

    explicit shader(GLuint program) noexcept;
    ~shader(void) noexcept;

	[[nodiscard]]
	GLuint get_program() noexcept;

	[[nodiscard]]
    shader::uniform get_uniform(const std::string& name);
	[[nodiscard]]
	shader::uniform get_uniform_block_index(const std::string &name);
    void activate() noexcept;
	void set_mat4(const std::string &name, glm::mat4 value);
	void set_int(const std::string &name, int value);
	void set_float(const std::string &name, float value);
	void set_vec3(const std::string &name, glm::vec3 value);
	void set_vec4(const std::string &name, glm::vec4 value);

private:
    unsigned int m_program{0};
};

}

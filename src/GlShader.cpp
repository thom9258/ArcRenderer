#include "../arc/GlShader.hpp"

#include <iostream>

namespace arc {

shader::shader(unsigned int program) noexcept
    : m_program(program) 
{
}

shader::~shader(void) noexcept 
{
    glDeleteShader(m_program);
}

std::unique_ptr<shader> shader::create(const char* vertex_source, const char* fragment_source) 
{
    unsigned int vShader;
    unsigned int fShader;
    int  success;
    char infoLog[512];

    vShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vShader, 1, &vertex_source, NULL);
    glCompileShader(vShader);
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vShader, 512, NULL, infoLog);
		std::cout << "Vertex Shader Compilaton Failed:\n"
				  << infoLog << std::endl;
		return nullptr;
    }
    
    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fragment_source, NULL);
    glCompileShader(fShader);
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fShader, 512, NULL, infoLog);
		std::cout << "Fragment Shader Compilaton Failed:\n"
				  << infoLog << std::endl;
		return nullptr;
    }

    /*Temporarily treat our shader as mutable at initialization*/
    GLuint program = glCreateProgram();
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
		std::cout << "Shader Program Linking Failed:\n"
				  << infoLog << std::endl;
		return nullptr;
    }

    /*clean up resources*/
    glDeleteShader(vShader);
    glDeleteShader(fShader); 
    return std::make_unique<shader>(program);
}
	
GLuint shader::get_program() noexcept
{
    return m_program;
}

void shader::activate() noexcept
{
    glUseProgram(m_program);
}

shader::uniform shader::get_uniform(const std::string &name)
{         
    const auto location = glGetUniformLocation(m_program, name.c_str());
    if (location == -1)
        return {};
    return {location};
}
	
shader::uniform shader::get_uniform_block_index(const std::string &name) 
{
	const auto location = glGetUniformBlockIndex(m_program, name.c_str());
    if (location == GL_INVALID_INDEX) {
        return std::nullopt;
	}
	return location;
}

void shader::set_mat4(const std::string &name, glm::mat4 value) {
    const auto location = get_uniform(name);
    if (!location)
		throw ARC_GENERIC_ERROR(name + " uniform does not exist!");
    glUniformMatrix4fv(*location, 1, GL_FALSE, glm::value_ptr(value)); 
}

void shader::set_int(const std::string &name, int value) {
    const auto location = get_uniform(name);
    if (!location)
		throw ARC_GENERIC_ERROR("Uniform '" + name + "' does not exist!");
    glUniform1i(*location, value); 
}

void shader::set_float(const std::string &name, float value) {
    const auto location = get_uniform(name);
    if (!location)
		throw ARC_GENERIC_ERROR("Uniform '" + name + "' does not exist!");
    glUniform1f(*location, value); 
}

void shader::set_vec3(const std::string &name, glm::vec3 value) {
    const auto location = get_uniform(name);
    if (!location)
		throw ARC_GENERIC_ERROR("Uniform '" + name + "' does not exist!");
    glUniform3f(*location, value.x, value.y, value.z); 
}

void shader::set_vec4(const std::string &name, glm::vec4 value) {
    const auto location = get_uniform(name);
    if (!location)
		throw ARC_GENERIC_ERROR("Uniform '" + name + "' does not exist!");
    glUniform4f(*location, value.x, value.y, value.z, value.w); 
}

}

#include "../arc/GlError.hpp"

namespace arc {

std::string codemark::string() const
{
	return "[L:" + std::to_string(line) + "  F:" + std::string(file) + "]";
}

generic_error::generic_error(const codemark where, const std::string what)
	: m_where(where)
	, m_what(what)
{
}

const std::string& generic_error::what()
{
	return m_what;
}

const char* gl_error_string(const GLenum err)
{
	switch (err) {
	case GL_NO_ERROR: return "GL_NO_ERROR";
	case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
	case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
	case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
	case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW";
	case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW";
	default:  return "GL_UNDEFINED_ERROR";
	}
}
	
void on_gl_error(const codemark where, std::function<void(const codemark, const GLenum)>&& f)
{
    const auto err = glGetError();
	if (err != GL_NO_ERROR)
		std::invoke(f, where, err);
}

}

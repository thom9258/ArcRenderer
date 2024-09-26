#pragma once
#include <glad/glad.h>

#include <string>
#include <functional>


#define ARC_CODEMARK {__FILE__, __LINE__}
#define ARC_GENERIC_ERROR(STR) arc::generic_error(ARC_CODEMARK, STR)
#define ARC_ON_GL_ERROR(DO) arc::on_gl_error(ARC_CODEMARK, DO)

namespace arc {
	
struct codemark {
	const char* file;
	const uint32_t line;
	std::string string() const;
};
	


struct generic_error {
	generic_error(const codemark where, const std::string what);
	codemark m_where;
	const std::string& what();
	std::string m_what;
};

const char* gl_error_string(const GLenum err);
	
void on_gl_error(const codemark where, std::function<void(const codemark, const GLenum)>&& f);
	
const auto throw_gl_error = [] (const codemark where, const GLenum err) {
	throw ARC_GENERIC_ERROR("GL got error ["
							+ where.string() + "] " 
							+ std::string(arc::gl_error_string(err)));
};

}	

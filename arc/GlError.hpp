#include <string>

#define arc_generic_error(STR) arc::generic_error({__FILE__, __LINE__}, STR)

namespace arc {
	
struct codemark {
	const char* file;
	const uint32_t line;
};

struct generic_error {
	generic_error(const codemark where, const std::string what);
	codemark m_where;
	const std::string& what();
	std::string m_what;
};

}	

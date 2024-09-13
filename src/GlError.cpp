#include "../arc/GlError.hpp"

namespace arc {

generic_error::generic_error(const codemark where, const std::string what)
	: m_where(where)
	, m_what(what)
{
}

const std::string& generic_error::what()
{
	return m_what;
}
	
}

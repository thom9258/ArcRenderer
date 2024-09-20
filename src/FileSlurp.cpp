#include "../arc/FileSlurp.hpp"

#include <fstream>
#include <streambuf>

namespace arc {

std::optional<std::string> file_slurp(const std::filesystem::path& path)
{
	std::error_code err;
	if (!std::filesystem::is_regular_file(path, err))
		return std::nullopt;

	std::ifstream t(path.string());
	std::string str;

	t.seekg(0, std::ios::end);   
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
				std::istreambuf_iterator<char>());

	return str;
}

}

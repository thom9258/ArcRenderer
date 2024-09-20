#pragma once


#include <optional>
#include <string>
#include <filesystem>

namespace arc {

[[nodiscard]]	
std::optional<std::string> file_slurp(const std::filesystem::path& path);

}

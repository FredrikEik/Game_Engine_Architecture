#include "FileSystemHelpers.h"
#include <regex>
#include <iostream>
std::filesystem::path FileSystemHelpers::extractRelativePath(const std::filesystem::path& path)
{
	std::wsmatch matches;
	std::regex pattern("((Assets\.*)|(Assets/.*))");
	std::smatch match;
	const std::string pathString = path.string();

	if (std::regex_search(pathString.begin(), pathString.end(), match, pattern))
		//for (const auto& it : match)
		return match[0].str();

	DEBUG_LOG("Could not extract relative path from path in FileSystemHelpers");

	return path;
}

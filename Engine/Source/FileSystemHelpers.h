#pragma once
#include <filesystem>
#include "CoreMinimal.h"
class FileSystemHelpers
{
public:
	static std::filesystem::path extractRelativePath(const std::filesystem::path& path);
	static bool getPathFromFileExplorer(std::string& OUTPath);
};


#include "FileSystemHelpers.h"
#include <regex>
#include <iostream>
#include "nfd.h"
std::filesystem::path FileSystemHelpers::extractRelativePath(const std::filesystem::path& path)
{
	std::wsmatch matches;
	std::regex pattern("((Assets\.*)|(Assets/*))");
	std::smatch match;
	const std::string pathString = path.string();

	if (std::regex_search(pathString.begin(), pathString.end(), match, pattern))
		//for (const auto& it : match)
		return match[0].str();

	DEBUG_LOG("Could not extract relative path from path in FileSystemHelpers");

	return path;
}

bool FileSystemHelpers::getPathFromFileExplorer(std::string& OUTPath)
{
	nfdchar_t* path = NULL;
	nfdresult_t result = NFD_OpenDialog(NULL, NULL, &path);
	if (result == NFD_OKAY) 
	{
		OUTPath = path;

		//ECS->addComponent<AxisAlignedBoxComponent>(entityID);
		//CollisionSystem::construct(entityID, ECS, true);
		//MeshComponent*  ECS->getComponentManager<MeshComponent>()->getComponentChecked();
		free(path);
		return true;
		//selectedEntity = -1;
		//std::cout << outPath;
	}
	else if (result == NFD_CANCEL) 
	{
		//puts("User pressed cancel.");
		return false;
	}
	else 
	{
		printf("FILESYSTEM ERROR: %s\n", NFD_GetError());
		return false;
	}


	return false;
}

bool FileSystemHelpers::getSavePathFromFileExplorer(std::string& OUTPath)
{
	nfdchar_t* path = NULL;
	nfdresult_t result = NFD_SaveDialog(NULL, NULL, &path);
	if (result == NFD_OKAY)
	{
		OUTPath = path;

		//ECS->addComponent<AxisAlignedBoxComponent>(entityID);
		//CollisionSystem::construct(entityID, ECS, true);
		//MeshComponent*  ECS->getComponentManager<MeshComponent>()->getComponentChecked();
		free(path);
		return true;
		//selectedEntity = -1;
		//std::cout << outPath;
	}
	else if (result == NFD_CANCEL)
	{
		//puts("User pressed cancel.");
		return false;
	}
	else
	{
		printf("FILESYSTEM ERROR: %s\n", NFD_GetError());
		return false;
	}

	return false;
}

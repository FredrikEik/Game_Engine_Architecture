#include "Factory.h"
#include "Assets/DefaultAssets.h"
#include <algorithm>

Factory::~Factory()
{
	delete componentManagers;
	delete& reusableAssetComponents;
}

Factory::ReusableAsset Factory::getReusableAsset(std::size_t hash)
{
	
	return reusableAssetComponents.at(hash);
}

uint32 Factory::loadAsset(uint32 entityID, DefaultAsset defaultAsset)
{
	std::cout << "Loading asset\n";
	switch (defaultAsset)
	{
<<<<<<< HEAD
	case asset_CUBE:
		return loadAsset(entityID, "Assets/cube.obj"); // Hardcoding filepath. Could generate as well
		break;
	case DefaultAsset::SPHERE:
		assert(false); // Implement it first, dummy
		break;
	case DefaultAsset::PLANE:
		assert(false); // Implement it first, dummy
		break;
	default:
		assert(false); // Trying to load asset that do not exist
		break;
	}

	return uint32();
}

// TODO: This is broken. "componentManagers->at(componentType)->removeComponent(entityID);" gets garbage values due to 
// acting on Component base class. Could maybe solve using the size of the type?
//void Factory::removeComponent(uint32 entityID, std::type_index componentType, uint32 componentID)
//{
//	assert(componentManagers->find(componentType) != componentManagers->end());
//
//	auto manager{ componentManagers->at(componentType) };
//	std::cout << "Removing component "<<componentType.name()<<'\n';
//	// If the components in this managers are reusable, do this
//	if (manager->bIsReusable)
//	{
//		// TODO: Uhhm, yeah. How about we don't iterate over an unodered_map?
//		// Please add some cheeky refactoring that makes this process not slow
//		// Maybe remove the branches while you're in the process as well, eh?
//		for (auto& it : reusableAssetComponents)
//		{
//			if (it.second.componentID == componentID)
//			{
//				auto entityArray = it.second.entitiesUsingAsset;
//
//				// If there is only one entity using the component, remove it instead
//				if (entityArray.size() <= 1)
//				{
//					reusableAssetComponents.erase(it.first);
//					break;
//				}
//
//				auto entityIterator = std::lower_bound(entityArray.begin(), entityArray.end(), entityID);
//				uint32 entityPosition = entityIterator - entityArray.begin();
//				assert(entityPosition < entityArray.size());
//
//				entityArray[entityPosition] = entityArray.back();
//				entityArray.pop_back();
//				std::sort(entityArray.begin(), entityArray.end());
//				return;
//			}
//		}
//	}
//	
//	componentManagers->at(componentType)->removeComponent(entityID);
//}



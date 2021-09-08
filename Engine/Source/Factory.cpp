#include "Factory.h"
#include "Assets/DefaultAssets.h"

uint32 Factory::loadAsset(uint32 entityID, DefaultAsset defaultAsset)
{
	switch (defaultAsset)
	{
	case asset_CUBE:
		return loadAsset(entityID, "../Assets/cube.obj"); // Hardcoding filepath. Could generate as well
		break;
	case asset_SPHERE:
		assert(false); // Implement it first, dummy
		break;
	case asset_PLANE:
		assert(false); // Implement it first, dummy
		break;
	default:
		assert(false); // Trying to load asset that do not exist
		break;
	}

	return uint32();
}
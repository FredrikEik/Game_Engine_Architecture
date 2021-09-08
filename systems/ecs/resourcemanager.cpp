#include "resourcemanager.h"

resourceManager::resourceManager()
{

}
MAX_ENTITIES_TYPE resourceManager::makeEntity(std::string meshName, TransformComponent transInput, std::string matName)
{
    unsigned int i = entities.size();
    entities.push_back(Entity(i));
    entities[i].mName = meshName;

    return i;

}

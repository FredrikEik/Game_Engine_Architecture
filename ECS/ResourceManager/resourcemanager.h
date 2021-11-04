#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <unordered_map>
#include "components.h"
#include "constants.h"

#include "ECS/ResourceManager/meshsystem.h"

class ResourceManager
{
    friend class ECScoordinator;

public:
    int ReadAsset(std::string AssetName);
    gsl::AssetType FindAssetType(std::string AssetName);

private:
    //Private constructor called from Create(), only possible from ECScoordinator.
    ResourceManager();

    static std::unique_ptr<ResourceManager> Create()
    { return std::unique_ptr<ResourceManager>( new ResourceManager() ); }

    //Data containers.
    std::unordered_map<std::string, class Texture*> m_textureMap;

    //Pointers for object systems.
    std::unique_ptr<MeshSystem> m_MeshHandler;
};

#endif // RESOURCEMANAGER_H

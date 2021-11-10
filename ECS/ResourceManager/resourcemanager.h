#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <unordered_map>
#include "components.h"
#include "constants.h"

namespace objl
{
    class Loader;
}

class ResourceManager
{
    friend class ECScoordinator;

public:
    int LoadAsset(std::string Filepath);
    gsl::AssetType FindAssetType(std::string AssetName);

private:
    //Private constructor called from Create(), only possible from ECScoordinator.
    ResourceManager();

    static std::unique_ptr<ResourceManager> Create()
    { return std::unique_ptr<ResourceManager>( new ResourceManager() ); }

    //Pointer to ".obj" loader.
    objl::Loader* m_Loader{nullptr};

};

#endif // RESOURCEMANAGER_H

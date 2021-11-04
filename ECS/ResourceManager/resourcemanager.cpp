#include "ECS/ResourceManager/resourcemanager.h"
#include "ECS/ResourceManager/meshsystem.h"

ResourceManager::ResourceManager()
{
    m_MeshHandler = std::make_unique<MeshSystem>();
}

int ResourceManager::ReadAsset(std::string AssetName)
{
    return 0;
}

gsl::AssetType ResourceManager::FindAssetType(std::string AssetName)
{
    if(AssetName.find(".obj") != std::string::npos)
        return gsl::OBJ;
    if (AssetName.find(".bmp") != std::string::npos)
       return gsl::BMP;
    if (AssetName.find(".wav") != std::string::npos)
       return gsl::WAV;

    return gsl::NULLASSETTYPE;
}

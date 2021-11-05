#include "ECS/ResourceManager/resourcemanager.h"

ResourceManager::ResourceManager()
{
    m_Loader = std::make_unique<objl::Loader>();
}

//Returns index for place in the appropriate data container. -1 if invalid/failed.
int ResourceManager::LoadAsset(std::string Filepath)
{
    gsl::AssetType type = FindAssetType(Filepath);


    if(type == gsl::OBJ)
    {
        m_Loader->LoadFile(Filepath);
        return m_Loader->LoadedMeshes.size()-1;
    }
    else if(type == gsl::BMP)
    {
        return -1; //Not implemented yet.
    }
    else if(type == gsl::WAV)
        return -1; //Not implemented yet.

    return -1;
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

#ifndef FACTORY_H
#define FACTORY_H

#include "iostream"
#include "gameobject.h"
#include <unordered_map>
#include <queue>
class Factory
{
public:
    Factory();
    std::unordered_map<uint32_t, GameObject*> mGameObjects;

    GameObject* createObject(std::string objectName);

    void saveMesh(std::string fileName, std::string nickName);


private:
    std::unordered_map<std::string, MeshComponent*> storedMeshes;
    std::queue<std::uint32_t> mAvailableIDs;


protected:
};

#endif // FACTORY_H

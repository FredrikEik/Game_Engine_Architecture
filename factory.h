#ifndef FACTORY_H
#define FACTORY_H

#include "iostream"
#include "gameobject.h"
#include "objreader.h"
#include <unordered_map>

class Factory
{
public:
    Factory();
    std::vector<GameObject*> mGameObjects;

    void createObject(std::string objectName);

    void saveMesh(std::string fileName, std::string nickName);


private:
    int resourceId;
    std::unordered_map<std::string, MeshComponent*> storedMeshes;


protected:
};

#endif // FACTORY_H

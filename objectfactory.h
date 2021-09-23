#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "iostream"
#include "gameobject.h"
#include "objimport.h"
#include <unordered_map>

class ObjectFactory
{
public:
    ObjectFactory();
    std::vector<GameObject*> mGameObject;
    void createObject(std::string objectName);

    void saveMesh(std::string fileName, std::string nickName);


    private:
        std::unordered_map<std::string, MeshComponent*> storedMeshes;


    protected:
};

#endif // OBJECTFACTORY_H

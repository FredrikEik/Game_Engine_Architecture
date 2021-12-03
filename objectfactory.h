#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "iostream"
#include "gameobject.h"
#include "objimport.h"
#include "planeimport.h"
#include <map>

class ObjectFactory
{
public:
    ObjectFactory();
    std::vector<GameObject*> mGameObject;
    void createObject(std::string objectName);

    void saveMesh(std::string fileName, std::string nickName);

    void setOBJindex(int index) { OBJindex = index; }
    int getOBJindex() { return OBJindex; }

    private:
        std::map<std::string, MeshComponent*> storedMeshes;

        int OBJindex;

        GameObject* p { nullptr };

        float count { 0.f };

    protected:
};

#endif // OBJECTFACTORY_H

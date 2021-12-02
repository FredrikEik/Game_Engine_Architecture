#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "iostream"
#include "gameobject.h"
#include "objimport.h"
#include "visualobject.h"
#include <map>

class ObjectFactory
{
public:
    ObjectFactory();
    std::vector<GameObject*> mGameObject;
    std::vector<VisualObject*> mVisualObjects;
    void createObject(std::string objectName);

    void saveMesh(std::string fileName, std::string nickName);

    void setOBJindex(int index) { OBJindex = index; }
    int getOBJindex() { return OBJindex; }

    int contourTick;
    GameObject* mSurface {nullptr};

    private:
        std::map<std::string, MeshComponent*> storedMeshes;

        int OBJindex;

    protected:
};

#endif // OBJECTFACTORY_H

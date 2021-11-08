#ifndef FACTORY_H
#define FACTORY_H

#include "iostream"
#include "gameobject.h"
#include <unordered_map>
#include <queue>
#include "level.h"

class Level;

class Factory
{
public:
    Factory();
    std::vector<GameObject*> mGameObjects;

    int cubecounter=0;
    int mariocounter=0;
    int trianglecounter=0;
    int spherecounter=0;
    int planecounter=0;
    int skyboxcounter=0;
    int cameracounter=0;

    GameObject* createObject(std::string objectName);

    void saveMesh(std::string fileName, std::string nickName);


    void openLevel(Level level);
private:
    std::unordered_map<std::string, MeshComponent*> storedMeshes;
    std::queue<std::uint32_t> mAvailableIDs;


protected:
};

#endif // FACTORY_H

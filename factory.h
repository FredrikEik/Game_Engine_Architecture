#ifndef FACTORY_H
#define FACTORY_H

#include "iostream"
#include "gameobject.h"
#include <unordered_map>
#include <queue>
#include <QOpenGLFunctions_4_1_Core>
#include "level.h"
#include "lassurface.h"

class Level;

class Factory : public QOpenGLFunctions_4_1_Core
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
    int lightCounter=0;
    int ballCounter=0;

    GameObject* createObject(gsl::objectTypes type);
    GameObject *createContourLines(GameObject *surfaceToContour);

    void saveMesh(std::string fileName, std::string nickName);
    void saveMesh(std::vector<Vertex> vertices, std::string nickName);
    void initMesh(MeshComponent* mesh);

    std::queue<std::uint32_t> mAvailableIDs;
    void openLevel(Level level);
    MeshComponent* getMeshFromMap(std::string name) {return storedMeshes[name];}

private:
    std::unordered_map<std::string, MeshComponent*> storedMeshes;



protected:
};

#endif // FACTORY_H

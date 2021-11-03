#ifndef FACTORY_H
#define FACTORY_H

#include "iostream"
#include "gameobject.h"
#include <unordered_map>
#include <queue>
#include <QOpenGLFunctions_4_1_Core>

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

    GameObject* createObject(std::string objectName);

    //Make mesh from .obj file
    void saveMesh(std::string fileName, std::string nickName);
    //Make mesh from vertices
    void saveMesh(std::vector<Vertex> vertices, std::string nickName);

    //Initialize mesh with vertices and indices
    void initMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, MeshComponent &mesh);
    //Initialize mesh with only vertices
    void initMesh(std::vector<Vertex> vertices, MeshComponent &mesh);


private:
    std::unordered_map<std::string, MeshComponent> storedMeshes;
    std::queue<std::uint32_t> mAvailableIDs;


protected:
};

#endif // FACTORY_H

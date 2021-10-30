#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "objreader.h"
#include "componenttypes.h"
#include <QOpenGLFunctions_4_1_Core>


class ResourceManager : public QOpenGLFunctions_4_1_Core
{
public:
    ResourceManager();

    void addMesh(std::string filePath);
    Mesh getMesh(std::string filePath);

    objReader OBJreader;

private:
                       //MeshName                 Pair <Vertecies and Indices>
    std::unordered_map<std::string, Mesh> storedMeshes;
    void initMesh(Mesh mesh, std::vector<Vertex> vertices);
};

#endif // RESOURCEMANAGER_H

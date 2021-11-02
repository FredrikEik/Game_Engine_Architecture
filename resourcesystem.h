#ifndef RESOURCESYSTEM_H
#define RESOURCESYSTEM_H

#include <QDebug>

#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <istream>
#include <sstream>
#include <vector>
#include <QDirIterator>
//#include <QFile>
#include <QFileInfo>
#include <rendersystem.h>
#include <QOpenGLFunctions_4_1_Core>

#include "components.h"
#include "constants.h"
#include "vertex.h"


struct meshData{
    std::vector<Vertex> meshVert;
    std::vector<GLuint> meshIndic;
    float collisionRadius;
    int internalIndex;
    GLuint VBO = 0;
    GLuint VAO = 0;
    GLuint DrawType = GL_TRIANGLES;
};
class resourceSystem
{
public:
    resourceSystem();
    void CreateMeshComponent();
    void CreateMeshComponent(std::string input, MeshComponent * mesh);
    std::vector<std::string> GetAllMeshesInAssetsDirectory();
    void ResourceSystemInit(RenderSystem * inRendSys);
    void SetMeshDataContainer();
    std::vector<Vertex> getVertexDataByName(std::string meshName);

    RenderSystem * rendSys;

    //frostum
    meshData* makeFrustum(const Frustum &frustumIn, RenderSystem * inRendSys);
private:
    std::vector < std::pair<std::string, std::vector<Vertex> > > meshContainer;
    std::vector < std::pair<std::string, std::vector<GLuint> > > meshIndiceContainer;
    std::vector < std::pair<std::string, float>> collisionRad;
    std::vector < std::pair<std::string, meshData>> meshDataContainer;
    float calculateLenght(QVector3D pos );

};

#endif // RESOURCESYSTEM_H

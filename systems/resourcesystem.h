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
#include <QFileInfo>
#include <rendersystem.h>
#include <QOpenGLFunctions_4_1_Core>

#include "components.h"
#include "constants.h"
#include "vertex.h"

/**
 * @brief The meshData struct - Mainly used as a model data structure. this is used to contain basic mesh data.
 */
struct meshData{
    std::vector<Vertex> meshVert;
    std::vector<GLuint> meshIndic;
    float collisionRadius;
    int internalIndex;
    GLuint VBO = 0;
    GLuint VAO = 0;
    GLuint DrawType = GL_TRIANGLES;
};

/**
 * @brief The resourceSystem class - The class that gets all necessary assets and data required for our object factory to properly function.
 */
class resourceSystem
{
public:
    /**
     * @brief resourceSystem - Empty Constructor.
     */
    resourceSystem();

    /**
     * @brief CreateMeshComponent - Gets correct mesh data from mesh data container and modifies a mesh component appropriately.
     * @param input - Mesh to get from mesh data container.
     * @param mesh - Mesh component to modify
     */
    void CreateMeshComponent(std::string input, MeshComponent * mesh);

    /**
     * @brief GetAllMeshesInAssetsDirectory - Gets all model assets path in directory.
     * @return vector of string paths.
     */
    std::vector<std::string> GetAllMeshesInAssetsDirectory();

    /**
     * @brief ResourceSystemInit - Initializes the resource system. Sets rendersystem pointer for later use in class.
     * @param inRendSys - rendersystem instance pointer.
     */
    void ResourceSystemInit(RenderSystem * inRendSys);

    /**
     * @brief SetMeshDataContainer - This gets all assets in directory and maps them to our struct model and then inserts them into our.
     */
    void SetMeshDataContainer();

    /**
     * @brief SetIntoMeshDataContainerRUNTIME - A function to set objects and their data into an model struct and init during runtime.
     * @param mVertices - data to init.
     * @param fname - what name to assign to the data.
     */
    void SetIntoMeshDataContainerRUNTIME(std::vector<Vertex> mVertices, std::string fname);

    /**
     * @brief getVertexDataByName - mainly used for physics and other calculations that are necessary.
     * @param meshName - target mesh name to get data for.
     * @return Vertex data.
     */
    std::vector<Vertex> getVertexDataByName(std::string meshName);

    /**
     * @brief CheckLOD12Presence - Checks for if target model in assets have LOD variants
     * @param meshName - model to check LOD models for.
     * @return true if both LOD level 1 and 2 are present, else its false.
     */
    bool CheckLOD12Presence(std::string meshName);

    /**
     * @brief getPureName - Filter function. we use this to just filter the .obj and what not from the name-
     * @param objMeshName - Object name from path
     * @return pure name.
     */
    std::string getPureName(std::string objMeshName);

    /**
     * @brief rendSys - Used to access the active instance of rendersystem. used to init.
     */
    RenderSystem * rendSys;


    /**
     * @brief makeFrustum - Creates the frustum wireframe.
     * @param frustumIn - Needed to access frustum data for calculations.
     * @param inRendSys - required to init the mesh and get a VAO index that can be used to render the mesh.
     * @return a meshData object with all the created data.
     */
    meshData* makeFrustum(const Frustum &frustumIn, RenderSystem * inRendSys);

    /**
     * @brief makeSkyBox - Creates the skybox mesh.
     * @param inRendSys - required to init the mesh and get a VAO index that can be used to render the mesh.
     * @return a meshData object with all the created data.
     */
    meshData* makeSkyBox( RenderSystem * inRendSys);
private:
    /**
     * @brief meshDataContainer - Contains all our object datas in a neat vector with a pair of string and meshdata. (we can refer to the object by object name. an index is provided but can be hard to use, however possible)
     */
    std::vector < std::pair<std::string, meshData>> meshDataContainer;


    float calculateLenght(QVector3D pos );

};

#endif // RESOURCESYSTEM_H

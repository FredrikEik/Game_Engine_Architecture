#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "GameObject.h"
#include "Components.h"
#include <map>
#include <vector>
#include <string>
#include "renderwindow.h"
#include "coreengine.h"
#include "texture.h"

class CoreEngine;

class ResourceManager : public QOpenGLFunctions_4_1_Core
{
public:

    class GameObject* CreateMainCharacter(std::string filename);
    class GameObject* CreateObject(std::string filename);

    static ResourceManager& getInstance();
    void init(Mesh &mesh, int lod);

    float getHeightMapHeight(const gsl::Vector2D& pos);
    gsl::Vector3D getHeightMapPosition();

    GameObject* mTerrain{nullptr};

    double scale = 0;

private:
    /** Functions */
    ResourceManager();

    int makeHeightMapFromTxt(std::string filename);
    int makeHeightMap(std::string filename);
    int contourLines();

    gsl::Vector3D barycentricCoordinates(const gsl::Vector2D& dette, const gsl::Vector2D& p1,
                                         const gsl::Vector2D& p2, const gsl::Vector2D& p3);

    void makeSphereRadius(Mesh* meshIn, gsl::Vector3D &vertexIn);

    int readObj(std::string filename);

/** Variables */
    GameObject* object;

    std::map<std::string, unsigned int> mMeshIndexMap;
    std::vector<Mesh> mMeshComponents;

    Mesh* mesh{nullptr};
    Transform* transform{nullptr};
    Material* material{nullptr};
    Texture* texture{nullptr};

    CoreEngine* mCoreEngine{nullptr};

    // HeightMap Variables
    gsl::Vector3D HeightMapPos{0,0,0};
    float *sArrayHeights{nullptr};
    float xyScale = 1;

    int mRows = 0;
    int mCols = 0;
    float zMin = 0.f, zMax = 0.f;
    float xStep = 0;
    float zStep = 0;
};

#endif // RESOURCEMANAGER_H

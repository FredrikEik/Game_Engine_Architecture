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

private:
    /** Functions */
    ResourceManager();

    int makeHeightMapFromTxt(std::string filename);
    int makeHeightMap(std::string filename);
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

    /** HeightMap Variables*/
    gsl::Vector3D HeightMapPos{0,0,0};
    float *sArrayHeights{nullptr};
    float xyScale = .1f;
    int rows{0};
    int cols{0};

};

#endif // RESOURCEMANAGER_H

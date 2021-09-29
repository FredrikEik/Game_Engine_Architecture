#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QOpenGLFunctions_4_1_Core>
#include <string>
#include <map>

#include "Components.h"
#include <vector>
#include "systems/ecs/Components.h"
#include "systems/ecs/entity.h"
#include <unordered_map>
#include "renderwindow.h"
#include "systems/ecs/meshhandeler.h"

//klasse som manager resurser og components

class Entity;
class MeshHandler;
struct MeshData;
class resourceManager : public QOpenGLFunctions_4_1_Core
{
public:

    static resourceManager& getInstance();
    void setOBBCollider(MAX_ENTITIES_TYPE entityID, gsl::Vector3D min = {0, 0, 0}, gsl::Vector3D max = {0, 0, 0});

    bool testCollision(Entity* a, Entity *b);

    void updateColliders();

     class Entity * makeEntity(std::string meshName);
    //Entity getEntety(size_t index) const{return entities[index];}

     bool hit = false;
     void CollidedRM(){hit = true;};
     gsl::Vector3D BoundingBoxMin;
     gsl::Vector3D BoundingBoxMax;
     bool checkCollideRM(gsl::Vector3D Pmin, gsl::Vector3D Pmax);
     void setBoundingBoxRM(gsl::Vector3D Min,gsl::Vector3D Max);

     MeshData makeLineBox(std::string meshName);
     MeshData makeCircleSphere(float radius, bool rgbColor);
     MeshHandler* mMeshHandler{nullptr};

    private:
    resourceManager();
    ~resourceManager();
    void operator=(resourceManager&){};
    resourceManager(const resourceManager&){};
//    void setMat(MAX_ENTITIES_TYPE entityId, gsl::Matrix4x4 TransformMatrix);
//    void setTransform(MAX_ENTITIES_TYPE entityID, gsl::Matrix4x4 TMatrix);
//    void setMesh(MAX_ENTITIES_TYPE entityID, std::string meshName);
//    void initMesh(MeshComponent & tempMeshComp);
//    int readObj(std::string filename);
//    int makeAxis();
//    int makeTriangle();
//    int makeCube();
    void makeOBB(gsl::Vector3D &vertexIn);

    void attachBoxCollider(short EntityNr, std::string objectType, gsl::Vector3D maxPoint = gsl::Vector3D(0,0,0), gsl::Vector3D minPoint = gsl::Vector3D(0,0,0));




    RenderWindow *mRenderwindow;
    gsl::Vector3D mMinCenter{-.5f, -.5f, -.5f};     //xyzMin if box, center if sphere
    gsl::Vector3D mMaxRadius{.5f, .5f, .5f};


    std::map<std::string, unsigned int> mMeshComponentMap;
    std::vector<MeshComponent> mMeshComponents;

};

#endif // RESOURCEMANAGER_H

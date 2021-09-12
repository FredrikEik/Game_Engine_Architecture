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

//klasse som manager resurser og components
class resourceManager : public QOpenGLFunctions_4_1_Core
{
public:

    static resourceManager& getInstance();


     class Entity * makeEntity(std::string meshName);
    //Entity getEntety(size_t index) const{return entities[index];}

    private:
    resourceManager();
    ~resourceManager();
    void operator=(resourceManager&){};
    resourceManager(const resourceManager&){};
//    void setMat(MAX_ENTITIES_TYPE entityId, gsl::Matrix4x4 TransformMatrix);
//    void setTransform(MAX_ENTITIES_TYPE entityID, gsl::Matrix4x4 TMatrix);
//    void setMesh(MAX_ENTITIES_TYPE entityID, std::string meshName);
    void initMesh(MeshComponent & tempMeshComp);
    int readObj(std::string filename);
    int makeAxis();
    int makeTriangle();
    int makeCube();


//    std::vector<gsl::Components> components;
//    std::vector<Entity> entities;
    std::map<std::string, unsigned int> mMeshComponentMap;
    std::vector<MeshComponent> mMeshComponents;

};

#endif // RESOURCEMANAGER_H

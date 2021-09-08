#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <vector>
#include "systems/ecs/Components.h"
#include "systems/ecs/entity.h"
#include <unordered_map>

//klasse som manager resurser og components
class resourceManager
{
public:
    resourceManager();
    Entity getEntety(size_t index) const{return entities[index];}
    //lage en drop down ui der man kan spawne entities
    MAX_ENTITIES_TYPE makeEntity(std::string meshName, TransformComponent transInput, std::string matName);
    void setMat(MAX_ENTITIES_TYPE entityId, gsl::Matrix4x4 TransformMatrix);
    void setTransform(MAX_ENTITIES_TYPE entityID, gsl::Matrix4x4 TMatrix);
    void setMesh(MAX_ENTITIES_TYPE entityID, std::string meshName);


private:
    std::vector<gsl::Components> components;
    std::vector<Entity> entities;

};

#endif // RESOURCEMANAGER_H

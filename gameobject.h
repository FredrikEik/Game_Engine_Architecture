#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "components.h"

class GameObject: public QOpenGLFunctions_4_1_Core{

public:

    GameObject();
    virtual ~GameObject();
    virtual void init() = 0;
    virtual void draw() = 0;
    virtual void move(float x, float y, float z) = 0;

    uint32_t ID;
    std::string mObjectName{"NoName"};  //temp
    std::string mObjectType{"NoType"};

    TransformComponent* getTransformComponent(){return transformComp;}
    MeshComponent* getMeshComponent(){return meshComp;}
    MaterialComponent* getMaterialComponent(){return materialComp;}
    SphereCollisionComponent* getSphereCollisionComponent() {return sphereCollisionComp;}
    JavaScriptComponent* getScriptComponent() {return scriptComp;}

    void setMeshComponent(MeshComponent* newMeshComp){meshComp=newMeshComp;}

private:

    TransformComponent* transformComp;
    MeshComponent* meshComp;
    MaterialComponent* materialComp;
    SphereCollisionComponent* sphereCollisionComp;
    JavaScriptComponent* scriptComp;

};
#endif // GAMEOBJECT_H


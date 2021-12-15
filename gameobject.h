#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "components.h"

class GameObject: public QOpenGLFunctions_4_1_Core {
public:
    GameObject();
    virtual ~GameObject();
    virtual void init() = 0;
    virtual void draw() = 0;
    virtual void move(float x, float y, float z) = 0;

    uint32_t ID;
    std::string mObjectName{"NoName"};  //temp
	gsl::ObjectType mObjectType{gsl::CUBE};
    bool isHUD{false};

    TransformComponent* getTransformComponent(){return transformComp;}
    MeshComponent* getMeshComponent(){return meshComp;}
    MaterialComponent* getMaterialComponent(){return materialComp;}

    void setMeshComponent(MeshComponent* newMeshComp){meshComp=newMeshComp;}

    SphereCollisionComponent* getSphereCollisionComponent() {return sphereCollisionComp;}

    ///Checks if an object is far enough away to activate lower res mesh
    void checkLodDistance(gsl::Vector3D distance, float cameraDistance);
    ///Bool for whether a low res mesh should be loaded
    bool BadLod{true};

private:
    TransformComponent* transformComp;
    MeshComponent* meshComp;
    MaterialComponent* materialComp;
    SphereCollisionComponent* sphereCollisionComp;

};
#endif // GAMEOBJECT_H


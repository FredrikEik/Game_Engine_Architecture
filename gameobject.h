#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "components.h"

class GameObject : public QOpenGLFunctions_4_1_Core
{
public:
    GameObject();
    virtual ~GameObject();
    virtual void init()=0;
    virtual void draw()=0;

    std::string mName;
    int mTexture = 0;

//    MeshComponent* MeshComp = nullptr;
    MeshComponent* getMeshComp(){return MeshComp;}
//    MaterialComponent* MaterialComp = nullptr;
    MaterialComponent* getMaterialComp(){return MaterialComp;}
//    TransformComponent* TransformComp = nullptr;
    TransformComponent* getTransformComp(){return TransformComp;}

    CollisionComponent* getCollisionComp(){return CollisionComp;}

    void setMeshComponent(MeshComponent* newMeshComp){MeshComp = newMeshComp;}

    MeshComponent* MeshComp;
    MaterialComponent* MaterialComp;
    TransformComponent* TransformComp;
    CollisionComponent* CollisionComp;

private:
};

#endif // GAMEOBJECT_H

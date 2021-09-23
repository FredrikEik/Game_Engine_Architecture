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

//    MeshComponent* MeshComp = nullptr;
    MeshComponent* getMeshComp(){return MeshComp;}
//    MaterialComponent* MaterialComp = nullptr;
    MaterialComponent* getMaterialComp(){return MaterialComp;}
//    TransformComponent* TransformComp = nullptr;
    TransformComponent* getTransformComp(){return TransformComp;}

    void setMeshComponent(MeshComponent* newMeshComp){MeshComp = newMeshComp;}

private:
    MeshComponent* MeshComp;
    MaterialComponent* MaterialComp;
    TransformComponent* TransformComp;
};

#endif // GAMEOBJECT_H

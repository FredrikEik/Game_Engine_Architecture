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

    TransformComponent* TransformComp = nullptr;
//    MeshComponent* MeshComp = nullptr;
    MeshComponent* getMeshComp(){return MeshComp;}
    MaterialComponent* MaterialComp = nullptr;

private:
    MeshComponent* MeshComp;
};

#endif // GAMEOBJECT_H

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

    TransformComponent* getTransformComponent(){return TransformComp;}
    MeshComponent* getMeshComponent(){return MeshComp;}
    TextureComponent* getMaterialComponent(){return TextureComp;}

    //private:
    TransformComponent* TransformComp;
    MeshComponent* MeshComp;
    TextureComponent* TextureComp;

};

#endif // GAMEOBJECT_H

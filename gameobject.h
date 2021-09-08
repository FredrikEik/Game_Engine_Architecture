#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "components.h"

class GameObject
{
public:
    GameObject();
    virtual ~GameObject();
    virtual void init();
    virtual void draw()=0;

    std::string mName;

    TransformComponent* TransformComp;
};

#endif // GAMEOBJECT_H

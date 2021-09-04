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

    std::string mName;

    MeshComponent* mesh;
    MaterialComponent* material;
    TransformComponent* transform;
};
#endif // GAMEOBJECT_H


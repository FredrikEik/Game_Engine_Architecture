#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "vertex.h"
#include "matrix4x4.h"
#include "shader.h"
#include "Components.h"

class RenderWindow;

class GameObject : public QOpenGLFunctions_4_1_Core {
public:
    GameObject();
    virtual ~GameObject();
    virtual void init();
    virtual void draw()=0;

    std::string mName;
    RenderWindow *mRenderWindow{nullptr}; //Just to be able to call checkForGLerrors()

    Mesh* mesh;             //MeshComponent
    Material* material;     //MaterialComponent
    Transform* transform;   //TransformComponent

};
#endif // GAMEOBJECT_H


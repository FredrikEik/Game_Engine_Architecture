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

    std::string mName{"null"};
    RenderWindow *mRenderWindow{nullptr}; //Just to be able to call checkForGLerrors()

    Mesh* mesh{nullptr};             //MeshComponent
    Material* material{nullptr};     //MaterialComponent
    Transform* transform{nullptr};   //TransformComponent

    void move(float dx, float dy, float dz);

};
#endif // GAMEOBJECT_H


#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "vertex.h"
#include "matrix4x4.h"
#include "shader.h"
#include "Components.h"

class RenderWindow;

/**
 * The GameObject class holds the name of the objects + pointers to all components needed
 * in a gameObject, it also has a move() function that is used when we need to change an objects position.
 * All other important values for the objects are in the components 'mesh', 'material', 'transform' and 'frustum'
 * structs in 'Components.h'.
 */
class GameObject : public QOpenGLFunctions_4_1_Core {
public:

    std::string mName{"null"};
    RenderWindow *mRenderWindow{nullptr}; //Just to be able to call checkForGLerrors()

    Mesh* mesh{nullptr};             //MeshComponent
    Material* material{nullptr};     //MaterialComponent
    Transform* transform{nullptr};   //TransformComponent
    Frustum* frustum{nullptr};

    void move(float dx, float dy, float dz);

};
#endif // GAMEOBJECT_H


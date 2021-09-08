#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "vertex.h"
#include "matrix4x4.h"
#include "shader.h"
#include "Components.h"

class RenderWindow;

class VisualObject : public QOpenGLFunctions_4_1_Core {
public:
    VisualObject();
    virtual ~VisualObject();
    virtual void init(); // GLint matrixUniform ????
    virtual void draw()=0;

    std::string mName;
    RenderWindow *mRenderWindow{nullptr}; //Just to be able to call checkForGLerrors()

    Mesh* mesh;             //MeshComponent
    Material* material;     //MaterialComponent
    Transform* transform;   //TransformComponent

};
#endif // VISUALOBJECT_H


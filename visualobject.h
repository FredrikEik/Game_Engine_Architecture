#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "vertex.h"
#include "matrix4x4.h"
#include "shader.h"
#include "components.h"

class RenderWindow;

class VisualObject : public QOpenGLFunctions_4_1_Core {
public:
    VisualObject();
    VisualObject(std::string name);
    virtual ~VisualObject();
    virtual void init();
    virtual void draw()=0;

    std::string mName;
    //uint ID;

    TransformComponent* mTransformComp{nullptr};
    MeshComponent* mMeshComp{nullptr};
    MaterialComponent* mMaterialComp{nullptr};
    //ShaderComponent* mShaderComp{nullptr};

    RenderWindow *mRenderWindow{nullptr}; //Just to be able to call checkForGLerrors()
};
static int ID;
#endif // VISUALOBJECT_H


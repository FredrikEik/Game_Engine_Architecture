#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H
#include "components.h"
#include <QOpenGLFunctions_4_1_Core>

class VisualObject : public QOpenGLFunctions_4_1_Core {
public:
    VisualObject();
    virtual ~VisualObject();
    virtual void init() = 0;
    //virtual void init(MeshComponent * mMesh) = 0;

    std::string mName;

    struct TransformComponent* mTransform{nullptr};
    struct MeshComponent* mMesh{nullptr};
    struct MaterialComponent* mMaterial{nullptr};
};
#endif // VISUALOBJECT_H


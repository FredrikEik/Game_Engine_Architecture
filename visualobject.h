#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include "component.h"
#include <QOpenGLFunctions_4_1_Core>
#include "mousepicking.h"

class VisualObject : public QOpenGLFunctions_4_1_Core {
public:
    VisualObject();
    virtual ~VisualObject();
    virtual void init();

    std::string mName;

    struct TransformComponent* mTransform{nullptr};
    struct MeshComponent* mMesh{nullptr};
    struct MaterialComponent* mMaterial{nullptr};
};
#endif // VISUALOBJECT_H

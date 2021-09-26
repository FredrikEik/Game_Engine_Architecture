#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <components.h>

class VisualObject : public QOpenGLFunctions_4_1_Core {
public:
    VisualObject();
    virtual ~VisualObject();
    virtual void init() = 0;

    std::string mName;
    int entitiyID = 0;

    TransformComponent* mTransform;
    MeshComponent* mMesh;
    MaterialComponent* mMaterial;
};
#endif // VISUALOBJECT_H

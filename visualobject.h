#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H
#include "component.h"
#include <sstream>
#include <QOpenGLFunctions_4_1_Core>
#include "collisionsystem.h"

class VisualObject : public QOpenGLFunctions_4_1_Core {
public:
    VisualObject();
    virtual ~VisualObject();
    virtual void init();
    virtual void move(float dx, float dy, float dz);
    void readFile(std::string filename);

    CollisionComponent* mCollision{nullptr};
    struct TransformComponent* mTransform{nullptr};
    struct MeshComponent* mMesh{nullptr};
    struct MaterialComponent* mMaterial{nullptr};
    struct NameComponent* mNameComp{nullptr};

private:
    CollisionSystem* mColSystem;
};
#endif // VISUALOBJECT_H

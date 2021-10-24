#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include "collisionsystem.h"
#include "component.h"
#include <QOpenGLFunctions_4_1_Core>
#include <sstream>


class VisualObject : public QOpenGLFunctions_4_1_Core {
public:
    VisualObject();
    virtual ~VisualObject();
    virtual void init();

   // std::string mName;
    CollisionComponent *mCollision{nullptr};
    virtual void move(float dx, float dy, float dz);
    void readFile(std::string filename);

    struct TransformComponent* mTransform{nullptr};
    struct MeshComponent* mMesh{nullptr};
    struct MaterialComponent* mMaterial{nullptr};
    struct NameComponent* mNameComp{nullptr};
private:
    CollisionSystem *mColSystem;
};
#endif // VISUALOBJECT_H

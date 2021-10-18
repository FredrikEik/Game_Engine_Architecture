#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include "collisionshapes.h"
#include <QOpenGLFunctions_4_1_Core>

class GameObject : public QOpenGLFunctions_4_1_Core
{
public:
    GameObject();

    std::string mName{"NoName"};

    virtual ~GameObject();
    virtual void init();
    virtual void draw();

    struct TransformComponent* mTransform{nullptr};
    struct MeshComponent* mMesh{nullptr};
    struct MaterialComponent* mMaterial{nullptr};
    class CollisionComponent* mCollision{nullptr};

};
#endif // VISUALOBJECT_H

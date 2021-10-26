#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

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
    struct AABB* mCollisionObject{nullptr};

};
#endif // GAMEOBJECT_H

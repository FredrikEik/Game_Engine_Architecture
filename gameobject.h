#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QOpenGLFunctions_4_1_Core>

class GameObject : public QOpenGLFunctions_4_1_Core {
public:
    GameObject();
    virtual ~GameObject();

    std::string mName;
    int entitiyID;

    struct TransformComponent* mTransform{nullptr};
    struct MeshComponent* mMesh{nullptr};
    struct MaterialComponent* mMaterial{nullptr};
    struct GravityComponent* mGravity{nullptr};
};
#endif // GAMEOBJECT_H


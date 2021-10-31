#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QOpenGLFunctions_4_1_Core>

class GameObject : public QOpenGLFunctions_4_1_Core
{
public:
    std::string mName{"DefaultName"};

    TransformComponent* mTransform{nullptr};
    MeshComponent* mMesh{nullptr};
    MaterialComponent* mMaterial{nullptr};
    SoundComponent* mSoundComponent{nullptr};
    PhysicsComponent* mPhysicsComponent{nullptr};
};
#endif // GAMEOBJECT_H


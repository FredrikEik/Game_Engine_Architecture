#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "components.h"

class GameObject : public QOpenGLFunctions_4_1_Core
{
public:
    GameObject();
    virtual ~GameObject();
    void init();
    void draw();

    std::string mName;

    MeshComponent* mMesh{nullptr};
    MaterialComponent* mMaterial{nullptr};
    TransformComponent* mTransform{nullptr};
    CollisionComponent* mCollision{nullptr};
    SoundComponent* mSoundComponent{nullptr};

    void setMeshComponent(MeshComponent* newMeshComp){mMesh = newMeshComp;}
};

#endif // GAMEOBJECT_H

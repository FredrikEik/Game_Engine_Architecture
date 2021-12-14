#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>

#include "components.h"
#include "meshhandler.h"

class MeshHandler;
class GameObject : public QOpenGLFunctions_4_1_Core {
public:
    std::string mName{"NoName"};
    QString objName{"NoName"};

     ColliderComponent *mCollider{nullptr};
     TransformComponent* mTransform{nullptr};
     MeshComponent* mMesh{nullptr};
     MaterialComponent* mMaterial{nullptr};
     SoundComponet* mSoundComponent{nullptr};

     MeshHandler* mMeshHandlerGameObject{nullptr};

     bool isAlive = true;
     bool ProjectileHit = false;
     bool ProjectileSpawned = false;
     bool particleDead = false;
     bool RenderInPlaymode = true;
     bool useFrustum = true;
     gsl::Vector3D startPos{0,0,0};

     GameObject* _las;
     void move(float x, float y, float z);
     void setSurface(GameObject* surface);
};
#endif // VISUALOBJECT_H


#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>

#include "components.h"

class GameObject : public QOpenGLFunctions_4_1_Core {
public:
     std::string mName{"NoName"};

     MAX_ENTITIES_TYPE mMaterialIDIndex;
     ecs::MaterialID mMaterialID;
     ecs::TransformComponent* mTransform{nullptr};
     ecs::MeshComponent* mMesh{nullptr};
     ecs::MaterialComponent* mMaterial{nullptr};
     ecs::SoundComponet* mSoundComponent{nullptr};

     void move(float x, float y, float z);


};
#endif // VISUALOBJECT_H


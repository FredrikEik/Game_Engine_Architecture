#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>

#include "components.h"

class GameObject : public QOpenGLFunctions_4_1_Core {
public:

     MAX_ENTITIES_TYPE mEntityID{MAX_ENTITIES};
     std::string mName{"NoName"};


     ecs::MaterialID mMaterialID;
     ecs::TransformComponent* mTransform{nullptr};
     ecs::MeshComponent* mMesh{nullptr};
     ecs::MaterialComponent* mMaterial{nullptr};
     ecs::SoundComponet* mSoundComponent{nullptr};

     void move(float x, float y, float z);


};
#endif // VISUALOBJECT_H


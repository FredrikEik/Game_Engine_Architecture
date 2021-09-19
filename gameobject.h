#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>

#include "components.h"

class GameObject : public QOpenGLFunctions_4_1_Core {
public:
    std::string mName{"NoName"};

     TransformComponent* mTransform{nullptr};
     MeshComponent* mMesh{nullptr};
     MaterialComponent* mMaterial{nullptr};
     SoundComponet* mSoundComponent{nullptr};
};
#endif // VISUALOBJECT_H


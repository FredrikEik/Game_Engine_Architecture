#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>

class GameObject : public QOpenGLFunctions_4_1_Core {
public:
    std::string mName{"NoName"};

    struct TransformComponent* mTransform{nullptr};
    struct MeshComponent* mMesh{nullptr};
    struct MaterialComponent* mMaterial{nullptr};
};
#endif // VISUALOBJECT_H


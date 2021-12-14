#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H
#include "component.h"
#include <sstream>
#include <QOpenGLFunctions_4_1_Core>
#include "collisionsystem.h"

/**
@class VisualObject - Dette er klassen de fleste objekter arver fra.
*Denne klassen inneholder alle component structene og init slik at alle objekter kan ha samme init.
 */

class VisualObject : public QOpenGLFunctions_4_1_Core {
public:
    VisualObject();
    virtual ~VisualObject();
    virtual void init();
    bool drawMe{true};

    struct CollisionComponent* mCollision{nullptr};
    struct TransformComponent* mTransform{nullptr};
    struct MeshComponent* mMesh{nullptr};
    struct MaterialComponent* mMaterial{nullptr};
    struct NameComponent* mNameComp{nullptr};



private:

    CollisionSystem* mColSystem;
};
#endif // VISUALOBJECT_H

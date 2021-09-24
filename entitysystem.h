#ifndef ENTITYSYSTEM_H
#define ENTITYSYSTEM_H
#include "components.h"
#include <QVector3D>
#include "meshsystem.h"

class EntitySystem
{
public:
    EntitySystem();
    ~EntitySystem();
    void construct(class RenderWindow * inRW,
                   int EntityId,
                   std::string ObjReader,
                   QVector3D StartPos,
                   GLuint shader,
                   GLint texture
                   );

    class RenderWindow * rw;
    TransformComponent *TransComp;
    MeshComponent *MeshComp;
    MaterialComponent *MatComp;
    meshsystem * MeshSys;
};

#endif // ENTITYSYSTEM_H

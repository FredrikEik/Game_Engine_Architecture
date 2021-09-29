#ifndef ENTITYSYSTEM_H
#define ENTITYSYSTEM_H
#include "components.h"
#include <QVector3D>
#include "meshsystem.h"
#include <vector>

class EntitySystem
{
public:
    EntitySystem();
    ~EntitySystem();
    void construct(class RenderWindow * inRW,
                   std::string ObjReader,
                   QVector3D StartPos,
                   GLuint shader,
                   GLint texture,
                   int EntityId = -1
                   );
    void constructCube();
    void constructSphere();
    void constructPlane();
    void constructSuzanne();

private:
    class RenderWindow * rw;
    TransformComponent *TransComp;
    MeshComponent *MeshComp;
    MaterialComponent *MatComp;
    meshsystem * MeshSys;
};

#endif // ENTITYSYSTEM_H

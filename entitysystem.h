#ifndef ENTITYSYSTEM_H
#define ENTITYSYSTEM_H
#include "components.h"
#include <QVector3D>
#include "resourcesystem.h"
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
    void constructCube(class RenderWindow * inRW);
    void constructSphere(class RenderWindow * inRW);
    void constructPlane(class RenderWindow * inRW);
    void constructSuzanne(class RenderWindow * inRW);
    void LODSuzanneSwithcer(MeshComponent* mesh);

private:
    class RenderWindow * rw;
    TransformComponent *TransComp;
    MeshComponent *MeshComp;
    MaterialComponent *MatComp;
    resourceSystem * ResourceSys;

    QVector3D LastPos = QVector3D(0.0f, 0.0f, 0.0f);
    void offsetLastPos();
};

#endif // ENTITYSYSTEM_H

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
    EntitySystem(class RenderWindow * inputRW);

    ~EntitySystem();
    void construct(std::string ObjReader,
                   QVector3D StartPos,
                   GLuint shader = 0,
                   GLint texture = 0,
                   int EntityId = -1,
                   GLenum drawType = GL_TRIANGLES
                   );
    void construcRay(QVector3D LineVec, QVector3D CameraPos, float Length = 25.0f);
    void constructCube();
    void constructSphere();
    void constructPlane();
    void constructSuzanne();
    void DestroyEntity();
    void LODSuzanneSwithcer(MeshComponent *mesh);
private:
    TransformComponent *TransComp;
    MeshComponent *MeshComp;
    MaterialComponent *MatComp;
    DetailsComponent * Deets;


    resourceSystem * ResourceSys;
    RenderWindow * inRW;


    QVector3D LastPos = QVector3D(0.0f, 0.0f, 0.0f);
    void offsetLastPos();
};

#endif // ENTITYSYSTEM_H

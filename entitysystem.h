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
    void construct(int EntityId,
                   std::vector<int>* entitivec,
                   TransformComponent* TransComp,
                   MeshComponent* MeshComp,
                   MaterialComponent* MatComp,
                   std::string ObjReader,
                   meshsystem * MeshSys,
                   std::vector<MeshComponent*> meshVec,
                   std::vector<TransformComponent*> transVec,
                   std::vector<MaterialComponent*> MatVec,
                   QVector3D StartPos,
                   GLuint shader,
                   GLint texture
                   );
};

#endif // ENTITYSYSTEM_H

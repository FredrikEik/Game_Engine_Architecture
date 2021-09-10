#include "entitysystem.h"

EntitySystem::EntitySystem()
{

}

EntitySystem::~EntitySystem()
{

}

void EntitySystem::construct(int EntityId, TransformComponent *TransComp, MeshComponent *MeshComp, MaterialComponent *MatComp, std::string ObjReader,meshsystem * MeshSys, std::vector<MeshComponent *> meshVec, std::vector<TransformComponent *> transVec, std::vector<MaterialComponent *> MatVec, QVector3D StartPos, GLuint shader, GLint texture)
{

    TransComp->mMatrix.setToIdentity();
    TransComp->entity = EntityId;
    TransComp->mMatrix.translate(StartPos.x(), StartPos.y(), StartPos.z());

    transVec.push_back(TransComp);

    MeshSys->CreateMeshComponent(ObjReader, MeshComp);
    MeshComp->entity = EntityId;
    meshVec.push_back(MeshComp);

    MatComp->entity = EntityId;
    MatComp->mShaderProgram = shader;
    MatComp->mTextureUnit = texture;
    MatVec.push_back(MatComp);

}

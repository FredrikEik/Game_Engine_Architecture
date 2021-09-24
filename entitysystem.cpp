#include "entitysystem.h"
#include "renderwindow.h"


EntitySystem::EntitySystem()
{
 MeshSys = new meshsystem();
}

EntitySystem::~EntitySystem()
{

}

void EntitySystem::construct(RenderWindow * inRW, int EntityId, std::string ObjReader, QVector3D StartPos, GLuint shader, GLint texture)
{
    rw = inRW;

    TransComp = new TransformComponent();
    MeshComp = new MeshComponent();
    MatComp = new MaterialComponent();

    TransComp->mMatrix.setToIdentity();
    TransComp->entity = EntityId;
    TransComp->mMatrix.translate(StartPos.x(), StartPos.y(), StartPos.z());

    rw->transformCompVec.push_back(TransComp);

    MeshSys->CreateMeshComponent(ObjReader, MeshComp);
    MeshComp->entity = EntityId;
    rw->meshCompVec.push_back(MeshComp);

    MatComp->entity = EntityId;
    MatComp->mShaderProgram = shader;
    MatComp->mTextureUnit = texture;
    rw->MaterialCompVec.push_back(MatComp);

    rw->entities.push_back(EntityId);
    rw->RenderSys->init(MeshComp);

}

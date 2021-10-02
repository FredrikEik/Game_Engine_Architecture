#include "entitysystem.h"
#include "renderwindow.h"


EntitySystem::EntitySystem()
{
 MeshSys = new resourceSystem();
}

EntitySystem::~EntitySystem()
{

}

void EntitySystem::construct(RenderWindow * inRW, std::string ObjReader, QVector3D StartPos, GLuint shader, GLint texture, int EntityId)
{



    rw = inRW;
    if(EntityId == -1){
        auto max = *std::max_element(rw->entities.begin(), rw->entities.end());
        EntityId = max + 1;
        rw->entities.push_back(EntityId);
    }else{
        bool matchCheck = false;
        int entitiesSize = rw->entities.size();
        for(int i = 0; i < entitiesSize; i++){
            if(rw->entities[i] == EntityId){
                matchCheck = true;
                //Give error message and assign new id.
                auto max = *std::max_element(rw->entities.begin(), rw->entities.end());
                EntityId = max + 1;
                rw->entities.push_back(EntityId);
                break;
            }
        }
        if(!matchCheck){
            rw->entities.push_back(EntityId);
        }
    }

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




    rw->RenderSys->init(MeshComp);

}

void EntitySystem::constructCube(class RenderWindow * inRW)
{
    offsetLastPos();
    if(inRW) construct(inRW,"cube.obj", LastPos,0,0);
}

void EntitySystem::constructSphere(class RenderWindow * inRW)
{
    offsetLastPos();
    if(inRW) construct(inRW,"sphere.obj", LastPos,0,0);
}

void EntitySystem::constructPlane(class RenderWindow * inRW)
{
    offsetLastPos();
    if(inRW) construct(inRW,"plane.obj", LastPos,0,0);
}

void EntitySystem::constructSuzanne(class RenderWindow * inRW)
{
    offsetLastPos();
    if(inRW) construct(inRW,"Suzanne.obj", LastPos,0,0);
}

void EntitySystem::offsetLastPos()
{
    float d = 1.5f; //delta
    LastPos.setX(LastPos.x() + d);
    //LastPos.setY(LastPos.y() + d);
    //LastPos.setZ(LastPos.z() + d);
}

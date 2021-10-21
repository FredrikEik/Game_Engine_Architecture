#include "entitysystem.h"
#include "renderwindow.h"


EntitySystem::EntitySystem()
{
 ResourceSys = new resourceSystem();
}

EntitySystem::~EntitySystem()
{

}

void EntitySystem::construct(RenderWindow * inRW, std::string ObjReader, QVector3D StartPos, GLuint shader, GLint texture, int EntityId)
{

    if(EntityId == -1){
        auto max = *std::max_element(inRW->entities.begin(), inRW->entities.end());
        EntityId = max + 1;
        inRW->entities.push_back(EntityId);
    }else{
        bool matchCheck = false;
        int entitiesSize = inRW->entities.size();
        for(int i = 0; i < entitiesSize; i++){
            if(inRW->entities[i] == EntityId){
                matchCheck = true;
                //Give error message and assign new id.
                auto max = *std::max_element(inRW->entities.begin(), inRW->entities.end());
                EntityId = max + 1;
                inRW->entities.push_back(EntityId);
                break;
            }
        }
        if(!matchCheck){
            inRW->entities.push_back(EntityId);
        }
    }

    TransComp = new TransformComponent();
    MeshComp = new MeshComponent();
    MatComp = new MaterialComponent();

    TransComp->mMatrix.setToIdentity();
    TransComp->entity = EntityId;
    TransComp->mMatrix.translate(StartPos.x(), StartPos.y(), StartPos.z());

    inRW->transformCompVec.push_back(TransComp);

    ResourceSys->CreateMeshComponent(ObjReader, MeshComp);
    MeshComp->entity = EntityId;
    MeshComp->centerOfMesh.setX(TransComp->mMatrix.getPosition().getX());
    MeshComp->centerOfMesh.setY(TransComp->mMatrix.getPosition().getY());
    MeshComp->centerOfMesh.setZ(TransComp->mMatrix.getPosition().getZ());
    inRW->meshCompVec.push_back(MeshComp);

    MatComp->entity = EntityId;
    MatComp->mShaderProgram = shader;
    MatComp->mTextureUnit = texture;
    inRW->MaterialCompVec.push_back(MatComp);


    inRW->RenderSys->init(MeshComp);

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

void EntitySystem::LODSuzanneSwithcer(MeshComponent *mesh)
{
    if(mesh->LODLevel == 0)
    {
        ResourceSys->CreateMeshComponent("Suzanne.obj", mesh);
        mesh->entity = 2;

    }
    else if (mesh->LODLevel == 1)
    {
        ResourceSys->CreateMeshComponent("suzanne_L01.obj", mesh);
        mesh->entity = 2;
    }
    else if(mesh->LODLevel == 2)
    {
        ResourceSys->CreateMeshComponent("suzanne_L02.obj", mesh);
        mesh->entity = 2;
    }

}
void EntitySystem::offsetLastPos()
{
    float d = 1.5f; //delta
    LastPos.setX(LastPos.x() + d);
    //LastPos.setY(LastPos.y() + d);
    //LastPos.setZ(LastPos.z() + d);
}
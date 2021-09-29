#include "entitysystem.h"
#include "renderwindow.h"


EntitySystem::EntitySystem()
{
 MeshSys = new meshsystem();
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

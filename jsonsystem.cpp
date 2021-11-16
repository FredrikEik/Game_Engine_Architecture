#include "jsonsystem.h"
#include "renderwindow.h"
#include "json.hpp"

using json = nlohmann::json;


JSONSystem::JSONSystem()
{
//    -Project Name:
//    -World Data:
//    1-Amount of objects to spawn
//    2.Camera Position, Angle etc
//    3-Object Data:
//        1- Name
//        2- Position
//        3- Texture/Shader Data
//        4- draw type.
//    4-Player Data:
//        1-PlayerPositon
}

void JSONSystem::JSONSystemInit(class RenderWindow * inRW)
{
    RW = inRW;
}

void JSONSystem::SaveLevel(std::string filepath)
{
    auto e    =   RW->entities;
    auto mC   =   RW->meshCompVec;
    auto tC   =   RW->transformCompVec;
    auto dC   =   RW->DeetsVector;
    auto matC =   RW->MaterialCompVec;

    json j;
    j = {{"fileSpecialisation" , "LEVEL"}, {"objectAmount",(int)e.size()}};
    for(int i = 0; i < (int)e.size(); i++){
//        for(int l = 0; l < (int)mC.size(); l++){
//            if(mC[l]->entity)
//        }
//        for(int k = 0; k < (int)mC.size(); k++){

//        }
        std::string name;
        float x,y,z;
        int shader, texture;

        for(int k = 0; k < (int)dC.size(); k++){
            if(dC[k]->entity == e[i]){
                name = dC[k]->title;
            }
        }
        for(int k = 0; k < (int)tC.size(); k++){
            if(tC[k]->entity == e[i]){
                x=tC[k]->mMatrix.getPosition().getX();
                y=tC[k]->mMatrix.getPosition().getY();
                z=tC[k]->mMatrix.getPosition().getZ();
            }
        }

        for(int k = 0; k < (int)matC.size(); k++){
            if(matC[k]->entity == e[i]){
                shader = matC[k]->mShaderProgram;
                texture = matC[k]->mTextureUnit;
            }
        }

        j["ObjectArr"].push_back({
                                 {"oName", name},
                                 {"oPosX", x},
                                 {"oPosY", y},
                                 {"oPosZ", z},
                                 {"oShader", shader},
                                 {"oTexture", texture}
                                 });

    }

    std::ofstream o(filepath);
    o << j.dump(4) << std::endl;
    o.close();
    //qDebug() << QString::fromStdString(j.dump(1));


}

void JSONSystem::LoadLevel(std::string filepath)
{

}

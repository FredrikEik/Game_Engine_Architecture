#include "jsonsystem.h"
#include "renderwindow.h"
#include "json.hpp"

using json = nlohmann::json;


JSONSystem::JSONSystem()
{

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
    std::ifstream i(filepath);
    json j;
    i >> j;

    int amountOfObjects = j.at("objectAmount");
    for(int i = 0; i < amountOfObjects; i++){
        RW->entitySys->construct(j.at("ObjectArr")[i].at("oName"),
                                 QVector3D(j.at("ObjectArr")[i].at("oPosX"),
                                           j.at("ObjectArr")[i].at("oPosY"),
                                           j.at("ObjectArr")[i].at("oPosZ")),
                                 j.at("ObjectArr")[i].at("oShader"),
                                 j.at("ObjectArr")[i].at("oTexture")
                                 );
    }

    qDebug() << "Amount Of Objects Loaded From "<< QString::fromStdString(filepath) << " :" << amountOfObjects;
}

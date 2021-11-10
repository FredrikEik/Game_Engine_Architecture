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

void JSONSystem::SaveLevel()
{
    json j= {
        {"fileSpecialisation", "LEVEL"},
        {"objectAmount", (int)RW->entities.size()}
      };

}

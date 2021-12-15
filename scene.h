#ifndef SCENE_H
#define SCENE_H

#include "resourcemanager.h"
#include "gameobject.h"
#include "rendersystem.h"

class ResourceManager;
class RenderSystem;
class Scene
{
public:
    Scene();
    void setupScene();
    ResourceManager *mResourceManager;

    GameObject *axis;
};

#endif // SCENE_H

#ifndef SCENE_H
#define SCENE_H

#include "resourcemanager.h"
#include "soundsystem.h"

#include "gameobject.h"
#include "rendersystem.h"
class ResourceManager;
class RenderSystem;
class scene
{
public:
    scene();
    void setupScene1();
    GameObject *axis;

private:

};

#endif // SCENE_H

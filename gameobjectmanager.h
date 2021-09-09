﻿#ifndef GAMEOBJECTMANAGER_H
#define GAMEOBJECTMANAGER_H

#include "xyz.h"
#include "camera.h"
#include "triangle.h"
#include "components.h"
#include "renderwindow.h"
#include "gameobject.h"

using namespace std;

//Camera *mCurrentCamera{nullptr};
//float mAspectratio{1.f};

//std::vector<GameObject*> mGameObjects;

void manageObjects();
void addObject(std::string name = "DefaultGameObject", int id = 0, int TransformID = 0, int MeshID = 0, int MaterialID = 0, int GravityID = 0);

//string objectName;

#endif // GAMEOBJECTMANAGER_H

#ifndef GAMEOBJECTMANAGER_H
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

//std::vector<VisualObject*> mVisualObjects;

void manageObjects();
int addObject(string name, int id, int TransformID, int MeshID, int MaterialID, int GravityID);

string objectName;

#endif // GAMEOBJECTMANAGER_H

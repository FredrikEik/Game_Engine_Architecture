#ifndef TRANSFORMSYSTEM_H
#define TRANSFORMSYSTEM_H
#include "vector3d.h"
class GameObject;

///Inspired by teamTOG's method of scaling and rotating
class TransformSystem
{
public:
    TransformSystem();

    static TransformSystem* getInstance();

    static void setPosition(GameObject* obj, gsl::Vector3D moveToPos);
    static void setScale(GameObject* obj, gsl::Vector3D scaleToValue);
    static void setRotation(GameObject* obj, gsl::Vector3D rotateToValue);
private:
    static void updateMatrix(GameObject* obj);
    static TransformSystem* mInstance;

    bool rotateOnce{true};
};

#endif // TRANSFORMSYSTEM_H

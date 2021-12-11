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

    void setPosition(GameObject* obj, gsl::Vector3D moveToPos);
    void setScale(GameObject* obj, gsl::Vector3D scaleToValue);
    void setRotation(GameObject* obj, gsl::Vector3D rotateToValue);
    void setCollisionRotation(GameObject* obj, float deg);
private:
    void updateMatrix(GameObject* obj);
    static TransformSystem* mInstance;

    bool rotateOnce{true};
};

#endif // TRANSFORMSYSTEM_H

#ifndef PLAYER_H
#define PLAYER_H

#include "gameobject.h"
#include "sphere.h"

class Player : public GameObject
{
public:
    Player();
    ~Player();

    void draw() override;
    void init() override;
    void move(float x, float y, float z) override;
    void movement(InputComponent* input);
    gsl::Vector3D cameraOffset{0.0f, 0.0f, -1.5f};
    gsl::Vector3D cameraTarget;
    gsl::Vector3D getCameraTarget () {return cameraTarget;}

    void setSurfaceToWalkOn(GameObject* surface){surfaceToWalkOn = surface;}
    void setPlayerCamera(GameObject* camera){playerCamera = camera;}
    private:
    GameObject* playerCamera = nullptr;
    GameObject* surfaceToWalkOn = nullptr;
    float movementSpeed = 0.3f;

};

#endif // PLAYER_H

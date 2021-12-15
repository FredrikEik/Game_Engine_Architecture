#ifndef PLAYER_H
#define PLAYER_H

#include "gameobject.h"
#include "sphere.h"
#include "camera.h"
///
/// \brief class for the  playable character
/// \author Jonas Bjørhei
/// \date 15/12/21
///
class Player : public GameObject
{
public:
    Player();
    ~Player();
    /// Draws the object
    void draw() override;
    /// Initiate the object
    void init() override;
    /// Translate the player
    void move(float x, float y, float z) override;
    ///Get input and call move() accordingly
    void movement(InputComponent* input);
    ///Makes the Player take damage, reacts to input, only implemented to showcase HUD
    int takeDamage(int &playerHP, InputComponent* input);
    ///Used to calculate camera target
    gsl::Vector3D cameraOffset     { 0.0f,  0.0f, -1.5f};
    ///The camera target position during game
    gsl::Vector3D cameraTarget     { 0.0f,  0.0f,  0.0f};
    ///Used to calculate new camera position
    gsl::Vector3D thirdPersonOffset{-3.0f,  2.0f,  0.0f};
    ///Calculate camera target
    gsl::Vector3D getCameraTarget () {return cameraTarget;}

    ///Makes the player walk on the surface
    void setSurfaceToWalkOn(GameObject* surface){surfaceToWalkOn = surface;}
    ///Set Player camera
    void setPlayerCamera(Camera* camera);

    Camera* playerCamera = nullptr;
    GameObject* surfaceToWalkOn = nullptr;
    ///Movement speed for the player
    float movementSpeed = 0.3f;

};

#endif // PLAYER_H

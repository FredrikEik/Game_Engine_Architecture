#ifndef PLAYER_H
#define PLAYER_H

#include "objectfactory.h"
#include "soundmanager.h"
#include "soundsource.h"


class player
{
public:
    player(GameObject* mesh);
    ~player();

    GameObject* mMesh { nullptr };

    void update(float deltaTime);

    void Move(float dt);
    void Jump(SoundSource* sound);

private:
    bool bCanJump { true };
    float mVelocity { 0.f };
    float mHeight { 0.51f };
    float mDt { 0.f };

};

#endif // PLAYER_H

#ifndef PLAYER_H
#define PLAYER_H

#include "objectfactory.h"


class player
{
public:
    player(GameObject* mesh);
    ~player();

    GameObject* mMesh { nullptr };

    void Move(float dt);

private:

};

#endif // PLAYER_H

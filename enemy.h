#ifndef ENEMY_H
#define ENEMY_H

#include "visualobject.h"


class Enemy : public VisualObject
{
public:
    Enemy(ShapeFactory* f);
    MovementComponent *mMoveComp{nullptr};
private:

    ShapeFactory* factoryPtr{nullptr};

};

#endif // ENEMY_H

#ifndef ENEMY_H
#define ENEMY_H

#include "visualobject.h"
/**
*Enemy - Ai som arver fra VisualObject.
*AI'en får nødvendige components som trengs i konstruktør.

 */

class Enemy : public VisualObject
{
public:
    Enemy(ShapeFactory* f);
    MovementComponent *mMoveComp{nullptr};
private:

    ShapeFactory* factoryPtr{nullptr};

};

#endif // ENEMY_H

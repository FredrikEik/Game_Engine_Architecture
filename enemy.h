#ifndef ENEMY_H
#define ENEMY_H

class Enemy : public VisualObject
{
public:
    Enemy(ShapeFactory* f);
    ~Enemy(){};
    MovementComponent* mMoveComp;
private:
    ShapeFactory* factoryPtr;
};

#endif // ENEMY_H

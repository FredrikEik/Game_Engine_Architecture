#ifndef ENEMY_H
#define ENEMY_H


class Enemy : public VisualObject
{
public:
    Enemy(ShapeFactory* f);
    ~Enemy(){};
    void moveEnemy();
    void rotateForwardV();
    bool checkMove;
    gsl::Vector3D mForward{0,0,1};
private:
    float speed = 0.025;
    ShapeFactory* factoryPtr;
    gsl::Vector3D dir{0,0,1};
};

#endif // ENEMY_H

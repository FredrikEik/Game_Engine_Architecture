#ifndef ENEMY_H
#define ENEMY_H


class Enemy : public VisualObject
{
public:
    Enemy(ShapeFactory* f);
    ~Enemy(){};
    void moveEnemy();
    bool checkMove;
    gsl::Vector3D mForward{0,0,-1};
private:
    float speed = 0.03;
    ShapeFactory* factoryPtr;
    gsl::Vector3D dir;
};

#endif // ENEMY_H

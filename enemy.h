#ifndef ENEMY_H
#define ENEMY_H


class Enemy : public VisualObject
{
public:
    Enemy(ShapeFactory* f);
    ~Enemy(){};
    void moveEnemy(Player *mPlayer, gsl::Vector3D Ppos);
    bool checkMove;
private:
    float speed = 0.03;
    ShapeFactory* factoryPtr;
    gsl::Vector3D dir;
};

#endif // ENEMY_H

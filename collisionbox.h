#ifndef COLLISIONBOX_H
#define COLLISIONBOX_H


class CollisionBox
{
public:
    CollisionBox();

    float cpxOne;
    float cpyOne;
    float cpxTwo;
    float cpyTwo;

    float cpxOneLocal;
    float cpyOneLocal;
    float cpxTwoLocal;
    float cpyTwoLocal;
};

#endif // COLLISIONBOX_H

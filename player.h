#ifndef PLAYER_H
#define PLAYER_H

#include "inputcomponent.h"
#include "visualobject.h"
#include "shapefactory.h"

class Player : public VisualObject
{
public:
    Player(ShapeFactory* f);
    ~Player();
    void movePlayer();
    void setForward(float dx, float dz);
    void centerPlayer();
    void CheckPlayerWall(Square* w);
    InputComponent* mInputComp;
//    bool onRwallX{false};
//    bool onLwallX{false};
//    bool onFwallY{false};
//    bool onBwallY{false};
    gsl::Vector3D mForward{0,0,-1};
private:
    float mx, my, mz; // posisjon
    CollisionSystem* mColSystem;
    ShapeFactory* factoryPtr;

};

#endif // PLAYER_H

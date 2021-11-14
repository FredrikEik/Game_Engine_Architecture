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
    virtual void move(float dx, float dy, float dz) override;
    void CheckPlayerWall(Square* w);
    InputComponent* mInputComp;
    bool onRwallX{false};
    bool onLwallX{false};
    bool onFwallY{false};
    bool onBwallY{false};
private:
    float mx, my, mz; // posisjon
    CollisionSystem* mColSystem;
    ShapeFactory* factoryPtr;

};

#endif // PLAYER_H

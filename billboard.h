#ifndef BILLBOARD_H
#define BILLBOARD_H

#include "gameobject.h"
#include "vector4d.h"
///
/// \brief class for the billboard/hud
/// \author Jonas Bjørhei
/// \date 15/12/21
///
class Billboard : public GameObject
{
public:
    Billboard();
    ~Billboard();
    ///Draws the billboard
    void draw() override;
    ///Initiate the billboard
    void init() override;
    ///Move the billboard
    void move(float x, float y, float z) override;

};

#endif // BILLBOARD_H

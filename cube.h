#ifndef CUBE_H
#define CUBE_H

#include "GameObject.h"

class Cube : public GameObject
{
public:
    Cube();
    void init() override;
    void draw() override;
private:


};

#endif // CUBE_H

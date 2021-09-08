#ifndef CUBE_H
#define CUBE_H

#include "visualobject.h"

class Cube : public VisualObject
{
public:
    Cube();
    void init() override;
    void draw() override;
private:


};

#endif // CUBE_H

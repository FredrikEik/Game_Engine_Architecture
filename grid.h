#ifndef GRID_H
#define GRID_H

#include "visualobject.h"

class grid : public VisualObject
{

private:


public:

    grid(int size, float spacing);
    grid();
   // ~grid();
    void init() override;
    void draw() override;
};

#endif // GRID_H

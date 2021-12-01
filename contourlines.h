#ifndef CONTOURLINES_H
#define CONTOURLINES_H

#include "gameobject.h"
#include "trianglesurface.h"

class ContourLines : public GameObject
{
public:
    ContourLines();
    ContourLines(GameObject* mGround, float height);
    ~ContourLines();

    void draw() override;
    void init() override;
    void move(float x, float y, float z) override;
};

#endif // CONTOURLINES_H

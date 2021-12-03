#ifndef CONTOURLINES_H
#define CONTOURLINES_H

#include "gameobject.h"
#include "planeimport.h"

class ContourLines : public GameObject
{
public:
    ContourLines(GameObject* plane, float height);
    ~ContourLines();

    void draw() override;
    void init() override;
};

#endif // CONTOURLINES_H

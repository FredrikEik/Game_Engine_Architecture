#ifndef CONTOURS_H
#define CONTOURS_H

#include "gameobject.h"
#include "surface.h"

class Contours : public GameObject
{
public:
    Contours();
    Contours(GameObject* mSurface, float height);
    ~Contours();

    void draw() override;
    void init() override;

};

#endif // CONTOURS_H

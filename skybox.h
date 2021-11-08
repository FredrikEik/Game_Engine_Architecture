#ifndef SKYBOX_H
#define SKYBOX_H

#include "cube.h"


class SkyBox : public Cube
{
public:
    SkyBox();
    ~SkyBox() override;

    void Update(gsl::Vector3D cameraPos);

    void draw() override;
    void init() override;
};

#endif // SKYBOX_H

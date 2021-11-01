#ifndef SKYBOX_H
#define SKYBOX_H

#include "visualobject.h"

class SkyBox : public VisualObject
{
public:
    SkyBox();
    virtual ~SkyBox() override;

    virtual void init(GLint matrixUniform) override;
    virtual void draw() override;

protected:
    class Cubemap* m_cubemap{};

};

#endif // SKYBOX_H

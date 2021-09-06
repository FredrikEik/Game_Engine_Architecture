#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "visualobject.h"

class LightSource : public VisualObject
{
public:
    LightSource(class Shader* shader);

    virtual void init(GLint matrixUniform) override;
    virtual void draw() override;

protected:
    float m_ambientStrengt{0.1f};
    float m_lightStrengt{0.7f};

    QVector3D m_lightColor{1.f, 1.f, 1.f};
    QVector3D m_lightPosition;

};

#endif // LIGHTSOURCE_H

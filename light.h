#ifndef LIGHT_H
#define LIGHT_H

#include "visualobject.h"
#include "matrix4x4.h"

/**
Light - Denne klassen holder nødenvige data for lys og konstruksjon av lys.
 */

class Light : public VisualObject
{
public:
    Light();
    ~Light() override;
    virtual void init() override;
    void makeVerticies(MeshComponent* dMesh);

    GLfloat mAmbientStrenght{0.3f};
    QVector3D mAmbientColor{0.3f, 0.3f, 0.3f};

    GLfloat mLightStrenght{0.7f};
    QVector3D mLightColor{1.0f, 1.0f, 1.0f};

     QVector3D mLightPosition{8,5,0};

    GLfloat mSpecularStrenght{0.3f};
    GLint mSpecularExponent{4};
};

#endif // LIGHT_H

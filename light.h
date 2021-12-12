#ifndef LIGHT_H
#define LIGHT_H

#pragma once

#include "gameobject.h"

class Light : public GameObject
{
public:
    Light();
    ~Light();

    void init() override;
    void draw() override;
    void move(float x, float y, float z) override;

    // Ambient
    GLfloat mAmbientStrength;
    gsl::Vector3D mAmbientColor;

    // Light
    GLfloat mLightStrength;
    gsl::Vector3D mLightColor;

    // Specular
    GLfloat mSpecularStrength;
    GLint mSpecularExponent;

    // Attenuation (falloff)
    GLfloat constant;
    GLfloat linear;
    GLfloat quadratic;

};

#endif // LIGHT_H

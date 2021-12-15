#ifndef PHYSICS_H
#define PHYSICS_H

#include "matrix4x4.h"
#include "vector3d.h"

/********************************************//**
* ... i have worked on this for the engine. most of it is taken from the simulation class,
* when i made physics for the rolling ball. thought it would be useful for the engine, but haven't
* implemented it anywhere yet
***********************************************/

class physics
{
public:
    physics();

    float Gravitation = 9.81;
    float friction = 0.009;
    float mass = 100;
    float radius = 0.25;

    gsl::Vector3D Acceleration{0.0, 0.0, -Gravitation};
    gsl::Vector3D Force{0.0,0.0,0.0};
    gsl::Vector3D Velocity{0,0,0};
    gsl::Vector3D OldVelocity{0,0,0};
    gsl::Vector3D airFlow{0,0,0};

    bool IsFalling{false};
    //funksjon for hvis objektet faller
    void Falling()
    {
        IsFalling = true;
        Acceleration = gsl::Vector3D(0, 0, -Gravitation);
        gsl::Vector3D u{0-Velocity.x,0-Velocity.y,0-Velocity.z};
        airFlow = gsl::Vector3D(u.x*u.x, u.y*u.y, u.z * u.z);
        Force = Acceleration * mass;
        Acceleration = {Force.x/mass, Force.y/mass, Force.z/mass};
    }
    //funksjon for hvis objectet er på bakken
    void onGround(gsl::Vector3D N)
    {
        if(IsFalling == true)
            OldVelocity.z = 0;
        IsFalling = false;
        Acceleration = gsl::Vector3D(N.x * N.z, N.y * N.z, (N.z*N.z)-1) * Gravitation;

    }
};

#endif // PHYSICS_H

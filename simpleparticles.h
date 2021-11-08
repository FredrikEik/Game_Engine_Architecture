#ifndef SIMPLEPARTICLES_H
#define SIMPLEPARTICLES_H

#include <QOpenGLFunctions_4_1_Core>
#include "matrix4x4.h"
#include "vector3d.h"
#include "shaderhandler.h"

class SimpleParticles
{
public:
    SimpleParticles();
};

class SimpleEmitter
{
public:
    SimpleEmitter();
    ~SimpleEmitter();

    void update( const float dt );

    void draw();

private:
    struct Particle
    {
        gsl::Vector3D position;
        float lifetime;
    };
    std::vector< Particle > particles;

    float positions[400];

    GLuint vertexBuffer;
    GLuint positionBuffer;
};



/*
shader
*/
class SimpleParticleShader : public ShaderHandler
{
public:
    SimpleParticleShader();
    ~SimpleParticleShader();

    void setUniformViewMatrix( const gsl::Matrix4x4 & M_v );
    void setUniformProjectionMatrix( const gsl::Matrix4x4& M_p );
    void setUniformParticleSize( const float particleSize );

private:
    static const std::string VS;
    static const std::string FS;

    GLuint u_M_v;
    GLuint u_M_p;
    GLuint u_particleSize;
};



/*
controller
*/
class SimpleController
{
public:
    SimpleController();
    ~SimpleController();

    void draw( const float dt , const gsl::Matrix4x4 M_p , const gsl::Matrix4x4& M_v );

private:
    SimpleParticleShader* shader;

    SimpleEmitter* emitter;
};
#endif // SIMPLEPARTICLES_H

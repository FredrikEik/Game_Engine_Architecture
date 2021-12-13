#ifndef PARTICLE_H
#define PARTICLE_H
#include <QOpenGLFunctions_4_1_Core>

#include "vector3d.h"
#include "matrix4x4.h"
#include "vertex.h"

class Particle : public QOpenGLFunctions_4_1_Core
{
public:
    Particle();
    Particle(gsl::Vector3D velocity, gsl::Vector3D position);
    gsl::Vector3D mVelocity{0,0,0};
    gsl::Vector3D mPosition{0,0,0};
    gsl::Matrix4x4 mMatrix;
    float gravity {-9.81f};
    float lifeLength{3};
    float elapsedTime{0};

    bool update();

    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;

    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0};

    GLenum mDrawType{GL_TRIANGLES};
private:

    void init();

};

#endif // PARTICLE_H

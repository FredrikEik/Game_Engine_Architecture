#ifndef LIGHT_H
#define LIGHT_H

#include <QOpenGLFunctions_4_1_Core>
#include <vertex.h>
#include "vertex.h"
#include "components.h"
#include "gameobject.h"


class light : public QOpenGLFunctions_4_1_Core
{
public:
    light();
    virtual void init();
    virtual void draw();

    GLfloat mAmbientStrength{0.5f};
    gsl::Vector3D mAmbientColor{0.3f, 0.3f, 0.3f};

    GLfloat mLightStrength{0.3f};
    gsl::Vector3D mLightColor{0.6f, 0.6f, 0.6f};

    GLfloat mSpecularStrength{0.3f};
    GLint mSpecularExponent{4};

    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;
    ecs::Material *mMaterial{nullptr};
    gsl::Matrix4x4 mMatrix;
    unsigned short mIndiceCount{0};

    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0}; //holds the indices (Element Array Buffer - EAB)

};

#endif // LIGHT_H

#ifndef SKYBOX_H
#define SKYBOX_H

#include <QOpenGLFunctions_4_1_Core>
#include "components.h"


class skybox : public QOpenGLFunctions_4_1_Core
{
public:
    //! \brief constructor for skybox
    //! \param vertex data
    skybox();
    ~skybox();
    //! \brief initializer skybox
    //! \param
    void init();
    //! \brief draw skybox
    void draw();
    //! \param helper functions
    GLuint skyID{0};
    GLuint mProgram{999};
    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;
    ecs::Material *mMaterial{nullptr};
    gsl::Matrix4x4 mMatrix;
    unsigned short mIndiceCount{0};

    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0}; //holds the indices (Element Array Buffer - EAB)
};

#endif // SKYBOX_H

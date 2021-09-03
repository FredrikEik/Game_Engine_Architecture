#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <QOpenGLFunctions_4_1_Core>

class Components
{
public:
    Components();
};

class TransformComponent : public Components
{

};

class MeshComponent : public Components
{
    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mEAB{0};
};

class TextureComponent : public Components
{

};

class CollisionComponent : public Components
{

};


#endif // COMPONENTS_H

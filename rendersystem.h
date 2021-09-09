#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H
#include "components.h"
#include <QOpenGLFunctions_4_1_Core>

class RenderSystem : public QOpenGLFunctions_4_1_Core{
public:
    RenderSystem();
    //virtual ~RenderSystem();
    void init(MeshComponent* mMesh);
    void draw(MeshComponent* mMesh);
};

#endif // RENDERSYSTEM_H



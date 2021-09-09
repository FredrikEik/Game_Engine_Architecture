#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H
#include "components.h"
#include "visualobject.h"

class RenderSystem : public VisualObject
{
public:
    RenderSystem();
    void init(MeshComponent* mMesh);
    void draw(MeshComponent* mMesh);
};

#endif // RENDERSYSTEM_H

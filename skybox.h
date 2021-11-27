#ifndef SKYBOX_H
#define SKYBOX_H
#include "stb_image.h"
#include "visualobject.h"
#include <vector>
#include "texture.h"

class Skybox : public VisualObject
{
public:
    Skybox();
    ~Skybox() override;
    void makeVerticies(MeshComponent* mMesh);
    Texture *mTextures[6]{nullptr};
    unsigned int textureID;
};

#endif // SKYBOX_H

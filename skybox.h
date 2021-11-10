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
    void setTexture();
    Texture *mTextures[6]{nullptr};
    unsigned int textureID;

private:

    std::string mFaces[6]
    {
        "../Skybox/right.bmp",
        "../Skybox/left.bmp",
        "../Skybox/bottom.bmp",
        "../Skybox/top.bmp",
        "../Skybox/front.bmp",
        "../Skybox/back.bmp"
    };
};

#endif // SKYBOX_H

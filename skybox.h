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
        "../GEA2021/Assets/skybox/right.bmp",
        "../GEA2021/Assets/skybox/left.bmp",
        "../GEA2021/Assets/skybox/bottom.bmp",
        "../GEA2021/Assets/skybox/top.bmp",
        "../GEA2021/Assets/skybox/front.bmp",
        "../GEA2021/Assets/skybox/back.bmp"
    };
};

#endif // SKYBOX_H


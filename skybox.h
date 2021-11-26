#ifndef SKYBOX_H
#define SKYBOX_H
#include "stb_image.h"
#include "visualobject.h"
#include <vector>
#include "texture.h"
//#include "texturehandler.h"

class Skybox : public VisualObject
{
public:
    Skybox(/*Texture *mTextures*/);
    ~Skybox() override;
    void makeVerticies(MeshComponent* mMesh);
    void setTexture();


 //  Texture *mTextures[6]{nullptr};

private:
//    std::string SkyboxName = "Skyb1";
//    std::string mFaces[6]
//    {
//        "skybox/Skyb1.bmp",
//        "skybox/Skyb2.bmp",
//        "skybox/Skyb3.bmp",
//        "skybox/Skyb4.bmp",
//        "skybox/Skyb5.bmp",
//        "skybox/Skyb6.bmp"
//    };
};

#endif // SKYBOX_H


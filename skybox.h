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

};


#endif // SKYBOX_H


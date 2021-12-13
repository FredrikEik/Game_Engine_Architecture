#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H
#include "gameobject.h"
#include "shader.h"

class PostProcessing : public GameObject
{
public:
    PostProcessing();
    ~PostProcessing();

    void draw() override{0;}
    void init() override{0;}
    void move(float x, float y, float z) override{0;}


    int createFramebuffer();
    int createTextureAttachment(int width, int height);
    void bindFramebuffer(int frameBuffer, int width, int height);
    void unbindCurrentFramebuffer();
    void hdr();

    int refractionFramebuffer;
    int refractionTexture;

    int getRefractionFramebuffer(){return refractionFramebuffer;}





};

#endif // POSTPROCESSING_H

#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H
#include "gameobject.h"
#include "shader.h"
#include "QOpenGLFunctions_4_1_Core"

class PostProcessing : public QOpenGLFunctions_4_1_Core
{
public:
    PostProcessing();
    PostProcessing(Shader *shader);
    ~PostProcessing();


    void createFramebuffer();
    void bindFramebuffer(int frameBufferer, int width, int height);
    void unbindCurrentFramebuffer();
    void hdr();
    void genQuad();
    unsigned int quadVAO, quadVBO;
    unsigned int framebuffer;
    unsigned int framebufferTexture;
    unsigned int RBO;

    int refractionFramebuffer;
    int refractionTexture;

    class Shader *shaderprog{nullptr};

    int getRefractionFramebuffer(){return refractionFramebuffer;}


};

#endif // POSTPROCESSING_H

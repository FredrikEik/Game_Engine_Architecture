#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H


class PostProcessing : protected QOpenGLFunctions_4_1_Core
{
public:
    PostProcessing();
    ~PostProcessing();

    int createFramebuffer();
    int createTextureAttachment(int width, int height);
    void bindFramebuffer(int frameBuffer, int width, int height);
    void unbindCurrentFramebuffer();

    int refractionFramebuffer;
    int refractionTexture;

    int getRefractionFramebuffer(){return refractionFramebuffer;}







};

#endif // POSTPROCESSING_H

#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H
#include "gameobject.h"
#include "shader.h"
#include "QOpenGLFunctions_4_1_Core"

class PostProcessing : public QOpenGLFunctions_4_1_Core
{
public:
///
///    \brief Class for making a custom framebuffer and post processing functions.
///    \author Paal Marius Haugen
///    \date 15/12/21
///
    ///Makes the quad for the custom framebuffer, sets the shader and run createFrameBuffer();
    PostProcessing(Shader *shader);
    ~PostProcessing();

    ///Binds frambuffer and texture to be rendered on quad
    void createFramebuffer();
    ///Binds the framebuffer in the start if each frame before drawing scene so it can take in the scene.
    void bindFramebuffer();
    ///Unbind the framebuffer with the scene, use the post processing effects and shaders, bind and draw quad with texture of scene
    void unbindCurrentFramebuffer();

    ///Generates a quad that we render to screen with a texture of the offscreen scene
    void genQuad();


    ///variables for the VAO, VBO, framebuffer, texture and render object
    unsigned int quadVAO, quadVBO;
    unsigned int framebuffer;
    unsigned int framebufferTexture;
    unsigned int RBO;

    class Shader *shaderprog{nullptr};

};

#endif // POSTPROCESSING_H

#include "postprocessing.h"

PostProcessing::PostProcessing()
{
    refractionFramebuffer = createFramebuffer();
    refractionTexture = createTextureAttachment(400,400);
}

PostProcessing::~PostProcessing()
{}

int PostProcessing::createFramebuffer()
{
    initializeOpenGLFunctions();

    GLuint framebuffer[1];
    glGenFramebuffers(1,framebuffer);
    glBindBuffer(GL_FRAMEBUFFER, framebuffer[0]);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    return framebuffer[0];
}

int PostProcessing::createTextureAttachment(int width, int height)
{
    initializeOpenGLFunctions();

    GLuint texture[1];
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); //(ByteBuffer) NULL i videoen
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture[0], 0);
    return texture[0];
}

void PostProcessing::bindFramebuffer(int frameBuffer, int width, int height)
{
    initializeOpenGLFunctions();

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glViewport(0, 0, width, height);
}

void PostProcessing::unbindCurrentFramebuffer()
{
    initializeOpenGLFunctions();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    QSize size = qApp->screens()[0]->size();
    glViewport(0,0, size.height(),size.width());
}

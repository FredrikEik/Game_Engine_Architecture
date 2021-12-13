#include "postprocessing.h"




PostProcessing::PostProcessing()
{
    refractionFramebuffer = createFramebuffer();
    refractionTexture = createTextureAttachment(720,480);
}

PostProcessing::~PostProcessing()
{}

int PostProcessing::createFramebuffer()
{
    initializeOpenGLFunctions();

    GLuint framebuffer{1};
    glGenFramebuffers(1, &framebuffer);
    glBindBuffer(GL_ARRAY_BUFFER, framebuffer);
    glDrawBuffer(GL_FRONT);
    return framebuffer;
}

int PostProcessing::createTextureAttachment(int width, int height)
{
    initializeOpenGLFunctions();

    GLuint texture{1};
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); //(ByteBuffer) NULL i videoen
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
    return texture;
}

void PostProcessing::bindFramebuffer(int frameBuffer, int width, int height)
{
    initializeOpenGLFunctions();

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glViewport(10, 10, width, height);
}

void PostProcessing::unbindCurrentFramebuffer()
{
    initializeOpenGLFunctions();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    QSize size = qApp->screens()[0]->size();
    glViewport(0,0, size.height(),size.width());
}

void PostProcessing::hdr()
{

    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, refractionTexture);

    unsigned int hdrFBO;
        glGenFramebuffers(1, &hdrFBO);
        // create floating point color buffer
        unsigned int colorBuffer;
        glGenTextures(1, &colorBuffer);
        glBindTexture(GL_TEXTURE_2D, colorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 480, 720, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // create depth buffer (renderbuffer)
        unsigned int rboDepth;
        glGenRenderbuffers(1, &rboDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 480, 720);
        // attach buffers
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            qDebug() << "Framebuffer not complete!";
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

#include "postprocessing.h"



PostProcessing::PostProcessing(Shader *shader)
{
    genQuad();
    shaderprog = shader;
    createFramebuffer();
}

PostProcessing::~PostProcessing()
{}

void PostProcessing::createFramebuffer()
{
    initializeOpenGLFunctions();


    float width = 1920;
    float height = 1080;
    // Create Frame Buffer Object
    glGenFramebuffers(1, &framebuffer);

    //This creates a color attachment
    glGenTextures(1, &framebufferTexture);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Create Render Buffer Object
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);//Bind it so its ready for storage
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    //attach the texture and renderbuffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);//Attach color texture
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);



    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
        qDebug() << "Framebuffer complete";

    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(fboStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        qDebug() << "Framebuffer error: " << fboStatus;
    }

}



void PostProcessing::bindFramebuffer()
{
    initializeOpenGLFunctions();

    // Bind the custom framebuffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
    glEnable(GL_DEPTH_TEST);    // Enable depth testing since it's disabled when drawing the framebuffer rectangle
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    // Clean the back buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



void PostProcessing::unbindCurrentFramebuffer()
{
    initializeOpenGLFunctions();

    // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
    // clear all relevant buffers
    glClear(GL_COLOR_BUFFER_BIT);

    shaderprog->use();
    shaderprog->setInt("screenTexture", 0);
    //


    glBindVertexArray(quadVAO);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);	// use the color attachment texture as the texture of the quad plane
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void PostProcessing::genQuad()
{
    initializeOpenGLFunctions();
    float quadVertices[] =
    {
       -1.0f,  1.0f,  0.0f, 1.0f,
       -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

       -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}





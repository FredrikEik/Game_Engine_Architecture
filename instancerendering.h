#ifndef INSTANCERENDERING_H
#define INSTANCERENDERING_H

/**
  Forsøk på instanced rendering.
  Hentet fra https://ogldev.org/www/tutorial33/tutorial33.html.
 */

class InstanceRendering
{
public:
    InstanceRendering();

public:

    void Render(unsigned int NumInstances, const gsl::Matrix4x4* WVPMats, const gsl::Matrix4x4* WorldMats);

private:

    #define INDEX_BUFFER 0
    #define POS_VB 1
    #define NORMAL_VB 2
    #define TEXCOORD_VB 3
    #define WVP_MAT_VB 4
    #define WORLD_MAT_VB 5

    GLuint m_VAO;
    GLuint m_Buffers[6];
};

#endif // INSTANCERENDERING_H

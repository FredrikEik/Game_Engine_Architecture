#include <math.h>

#include "light.h"
#include "components.h"

Light::Light()
{
    // Ambient
    mAmbientStrenght = 0.15f;
    mAmbientColor = gsl::Vector3D(0.8f, 0.8f, 0.15f);

    // Light
    mLightStrenght = 1.5f;
    mLightColor = gsl::Vector3D(0.8f, 0.8f, 0.15f);

    // Specular
    mSpecularStrenght = 0.7f;
    mSpecularExponent = 32;

    // Attenuation (falloff)
    constant = 1.0f;
    linear = 0.14f;
    quadratic = 0.07f;

//    ********* TABLE FOR ATTENUATION VALUES *********
//    Distance    Constant    Linear    Quadratic
//    7           1.0          0.7	     1.8
//    13          1.0	       0.35	     0.44
//    20          1.0	       0.22	     0.20
//    32          1.0	       0.14	     0.07
//    50          1.0	       0.09	     0.032
//    65          1.0	       0.07	     0.017
//    100         1.0	       0.045	 0.0075
//    160         1.0	       0.027	 0.0028
//    200         1.0	       0.022	 0.0019
//    325         1.0	       0.014	 0.0007
//    600         1.0	       0.007	 0.0002
//    3250        1.0	       0.0014	 0.000007

    // Vertices
    getMeshComponent()->mVertices.push_back(Vertex{-0.5f, -0.5f,  0.5f,    0.3f, 0.0f, 0.5f,    0.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{ 0.5f, -0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-0.5f,  0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});

    getMeshComponent()->mVertices.push_back(Vertex{ 0.5f,  0.5f,  0.5f,    0.7f, 0.0f, 0.3f,    1.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-0.5f,  0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{ 0.5f, -0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});

    getMeshComponent()->mVertices.push_back(Vertex{ 0.5f, -0.5f,  0.5f,    0.3f, 0.0f, 0.5f,    0.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{ 0.5f, -0.5f, -0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{ 0.5f,  0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});

    getMeshComponent()->mVertices.push_back(Vertex{ 0.5f,  0.5f, -0.5f,    0.7f, 0.0f, 0.3f,    1.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{ 0.5f,  0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{ 0.5f, -0.5f, -0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});

    getMeshComponent()->mVertices.push_back(Vertex{ 0.5f, -0.5f, -0.5f,    0.3f, 0.0f, 0.5f,    0.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-0.5f, -0.5f, -0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{ 0.5f,  0.5f, -0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});

    getMeshComponent()->mVertices.push_back(Vertex{-0.5f,  0.5f, -0.5f,    0.7f, 0.0f, 0.3f,    1.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{ 0.5f,  0.5f, -0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-0.5f, -0.5f, -0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});

    getMeshComponent()->mVertices.push_back(Vertex{-0.5f, -0.5f, -0.5f,    0.3f, 0.0f, 0.5f,    0.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-0.5f, -0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-0.5f,  0.5f, -0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});

    getMeshComponent()->mVertices.push_back(Vertex{-0.5f,  0.5f,  0.5f,    0.7f, 0.0f, 0.3f,    1.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-0.5f,  0.5f, -0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-0.5f, -0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});

    getMeshComponent()->mVertices.push_back(Vertex{-0.5f, -0.5f, -0.5f,    0.3f, 0.0f, 0.5f,    0.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{ 0.5f, -0.5f, -0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-0.5f, -0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});

    getMeshComponent()->mVertices.push_back(Vertex{ 0.5f, -0.5f,  0.5f,    0.7f, 0.0f, 0.3f,    1.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-0.5f, -0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{ 0.5f, -0.5f, -0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});

    getMeshComponent()->mVertices.push_back(Vertex{-0.5f,  0.5f,  0.5f,    0.3f, 0.0f, 0.5f,    0.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{ 0.5f,  0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-0.5f,  0.5f, -0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});

    getMeshComponent()->mVertices.push_back(Vertex{ 0.5f,  0.5f, -0.5f,    0.7f, 0.0f, 0.3f,    1.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-0.5f,  0.5f, -0.5f,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{ 0.5f,  0.5f,  0.5f,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});
}

Light::~Light()
{

}

void move(float x, float y, float z)
{

}

void Light::init()
{
    initializeOpenGLFunctions();

       // Set what shader you want to use to render this object
       //mMaterial->setActiveShader(ShaderType::TEXTURE_SHADER);
       //mMaterial->setActiveTextureSlot(2);
       //mMaterial->setupModelMatrixUniform(mMatrixUniform, matrixUniform);

       glGenVertexArrays( 1, &getMeshComponent()->mVAO );
       glBindVertexArray( getMeshComponent()->mVAO );


       glGenBuffers( 1, &getMeshComponent()->mVBO );
       glBindBuffer( GL_ARRAY_BUFFER, getMeshComponent()->mVBO );

       glBufferData( GL_ARRAY_BUFFER,                     //what buffer type
                     getMeshComponent()->mVertices.size() * sizeof( Vertex ), //how big buffer do we need
                     getMeshComponent()->mVertices.data(),                    //the actual vertices
                     GL_STATIC_DRAW                       //should the buffer be updated on the GPU
                     );

       glBindBuffer(GL_ARRAY_BUFFER, getMeshComponent()->mVBO);

       // 1rst attribute buffer : coordinates

       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(0));
       glEnableVertexAttribArray(0);

       // 2nd attribute buffer : colors
       glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)) );
       glEnableVertexAttribArray(1);

       // 3rd attribute buffer : uvs
       glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
       glEnableVertexAttribArray(2);

       glBindVertexArray(0);
}

void Light::draw()
{
    glBindVertexArray( getMeshComponent()->mVAO );
    glDrawArrays(GL_TRIANGLES, 0, getMeshComponent()->mVertices.size());
    glBindVertexArray(0);
}

void Light::move(float x, float y, float z)
{
    getTransformComponent()->mMatrix.translate(x,y,z);
}

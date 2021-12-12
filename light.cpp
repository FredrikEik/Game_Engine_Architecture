#include <math.h>

#include "light.h"
#include "components.h"
#include "renderwindow.h"

Light::Light()
{
    // Ambient
    mAmbientStrength = 0.15f;
    mAmbientColor = gsl::Vector3D(0.3f, 0.3f, 0.8f);

    // Light
    mLightStrength = 1.5f;
    mLightColor = gsl::Vector3D(0.8f, 0.8f, 0.8f);

    // Specular
    mSpecularStrength = 0.7f;
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
    getMeshComponent()-> mVertices.insert( getMeshComponent()-> mVertices.end(),
    {
        Vertex(-0.25f,  0.25f,  -0.25f,   0.8f, 0.8f, 0.8f,     0.f,  0.f),   //Left low
        Vertex( 0.25f,  0.25f,  -0.25f,   0.8f, 0.8f, 0.8f,     1.f,  0.f),   //Right low
        Vertex( 0.0f,   0.0f,    0.25f,   0.8f, 0.8f, 0.8f,     0.5f, 0.5f),  //Top
        Vertex( 0.0f,  -0.25f,  -0.25f,   0.8f, 0.8f, 0.8f,     0.5f, 0.5f)   //Back low
    });

    getMeshComponent()-> mIndices.insert( getMeshComponent()-> mIndices.end(),
    { 0, 1, 2,
      1, 3, 2,
      3, 0, 2,
      0, 3, 1
    });
}

Light::~Light()
{

}

void Light::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &getMeshComponent()->mVAO );
    glBindVertexArray( getMeshComponent()->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &getMeshComponent()->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, getMeshComponent()->mVBO );

    glBufferData( GL_ARRAY_BUFFER, getMeshComponent()->mVertices.size()*sizeof(Vertex), getMeshComponent()->mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, getMeshComponent()->mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)( 6 * sizeof( GLfloat ) ));
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &getMeshComponent()->mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getMeshComponent()->mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getMeshComponent()->mIndices.size() * sizeof(GLuint), getMeshComponent()->mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Light::draw()
{
    glBindVertexArray(getMeshComponent()-> mVAO );
    //glUniformMatrix4fv(getMeshComponent()-> mMatrixUniform, 1, GL_FALSE, getTransformComponent()-> mMatrix.constData());
    glDrawElements(GL_TRIANGLES, getMeshComponent()-> mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Light::move(float x, float y, float z)
{
    getTransformComponent()->mMatrix.translate(x,y,z);
    getSphereCollisionComponent()->center += gsl::Vector3D(x,y,z);
}

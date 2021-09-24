#include "bigplane.h"
#include "components.h"

BigPlane::BigPlane()
{
    getMeshComponent()->mVertices.push_back(Vertex{5.5, 0.0, 5.5, 5.5, 0.0, 5.5});
    getMeshComponent()->mVertices.push_back(Vertex{-5.5, 0.0, 5.5, -5.5, 0.0, 5.5});
    getMeshComponent()->mVertices.push_back(Vertex{-5.5, 0.0, -5.5, -5.5, 0.0, -0.5});

    getMeshComponent()->mVertices.push_back(Vertex{5.5, 0.0, 5.5, 5.5, 0.0, 5.5});
    getMeshComponent()->mVertices.push_back(Vertex{5.5, 0.0, -5.5, 5.5, 0.0, -5.5});
    getMeshComponent()->mVertices.push_back(Vertex{-5.5, 0.0, -5.5, -5.5, 0.0, -5.5});

    /* Collision in the future probably
    collisionBox->cpxOneLocal = collisionBox->cpxOne;
    collisionBox->cpyOneLocal = collisionBox->cpyOne;
    collisionBox->cpxTwoLocal = collisionBox->cpxTwo;
    collisionBox->cpyTwoLocal = collisionBox->cpyTwo;
    */

}
BigPlane::~BigPlane(){}

void BigPlane::init()
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();


    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &getMeshComponent()->mVAO );
    glBindVertexArray( getMeshComponent()->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &getMeshComponent()->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, getMeshComponent()->mVBO );

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER,
                  getMeshComponent()->mVertices.size()*sizeof( Vertex ),
                  getMeshComponent()->mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0  );          // array buffer offset
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void BigPlane::draw()
{
    glBindVertexArray(getMeshComponent()->mVAO );
    glDrawArrays(GL_TRIANGLES, 0, getMeshComponent()->mVertices.size());
    glBindVertexArray(0);
}

#include "triangle.h"
#include "vertex.h"

Triangle::Triangle()
{                                                   // Positions            // Colors       //UV
    getMeshComponent()->mVertices.push_back(Vertex{-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.f}); // Bottom Left
    getMeshComponent()->mVertices.push_back(Vertex{ 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.f}); // Bottom Right
    getMeshComponent()->mVertices.push_back(Vertex{ 0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.f}); // Top
}

Triangle::~Triangle()
{
}

void Triangle::init()
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();


    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &meshComp->mVAO );
    glBindVertexArray( meshComp->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &meshComp->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, meshComp->mVBO );

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER,
                  meshComp->mVertices.size()*sizeof( Vertex ),
                  meshComp->mVertices.data(), GL_STATIC_DRAW );

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

void Triangle::draw()
{
    glBindVertexArray(meshComp->mVAO );
    glDrawArrays(GL_TRIANGLES, 0, meshComp->mVertices.size());
    glBindVertexArray(0);
}

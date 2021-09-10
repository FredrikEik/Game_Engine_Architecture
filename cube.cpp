#include "cube.h"

Cube::Cube()
{
    //Bunn                      x   y   z    r   g   b
    mVertices.push_back(Vertex(0.0,0.0,0.0, 1.0,0.0,0.0));
    mVertices.push_back(Vertex(1.0,0.0,0.0, 1.0,0.0,0.0));
    mVertices.push_back(Vertex(1.0,0.0,1.0, 1.0,0.0,0.0));
    mVertices.push_back(Vertex(1.0,0.0,1.0, 1.0,0.0,0.0));
    mVertices.push_back(Vertex(0.0,0.0,1.0, 1.0,0.0,0.0));
    mVertices.push_back(Vertex(0.0,0.0,0.0, 1.0,0.0,0.0));
    //topp
    mVertices.push_back(Vertex(0.0,1.0,0.0, 1.0,1.0,0.0));
    mVertices.push_back(Vertex(1.0,1.0,0.0, 1.0,1.0,0.0));
    mVertices.push_back(Vertex(1.0,1.0,1.0, 1.0,1.0,0.0));
    mVertices.push_back(Vertex(1.0,1.0,1.0, 1.0,1.0,0.0));
    mVertices.push_back(Vertex(0.0,1.0,1.0, 1.0,1.0,0.0));
    mVertices.push_back(Vertex(0.0,1.0,0.0, 1.0,1.0,0.0));
    //Venstre
    mVertices.push_back(Vertex(0.0,0.0,0.0, 0.0,1.0,0.0));
    mVertices.push_back(Vertex(0.0,1.0,0.0, 0.0,1.0,0.0));
    mVertices.push_back(Vertex(0.0,1.0,1.0, 0.0,1.0,0.0));
    mVertices.push_back(Vertex(0.0,1.0,1.0, 0.0,1.0,0.0));
    mVertices.push_back(Vertex(0.0,0.0,1.0, 0.0,1.0,0.0));
    mVertices.push_back(Vertex(0.0,0.0,0.0, 0.0,1.0,0.0));
    //Høyre
    mVertices.push_back(Vertex(1.0,0.0,0.0, 0.0,1.0,1.0));
    mVertices.push_back(Vertex(1.0,1.0,0.0, 0.0,1.0,1.0));
    mVertices.push_back(Vertex(1.0,1.0,1.0, 0.0,1.0,1.0));
    mVertices.push_back(Vertex(1.0,1.0,1.0, 0.0,1.0,1.0));
    mVertices.push_back(Vertex(1.0,0.0,1.0, 0.0,1.0,1.0));
    mVertices.push_back(Vertex(1.0,0.0,0.0, 0.0,1.0,1.0));
    //Bak
    mVertices.push_back(Vertex(0.0,0.0,0.0, 0.0,0.0,1.0));
    mVertices.push_back(Vertex(1.0,0.0,0.0, 0.0,0.0,1.0));
    mVertices.push_back(Vertex(1.0,1.0,0.0, 0.0,0.0,1.0));
    mVertices.push_back(Vertex(1.0,1.0,0.0, 0.0,0.0,1.0));
    mVertices.push_back(Vertex(0.0,1.0,0.0, 0.0,0.0,1.0));
    mVertices.push_back(Vertex(0.0,0.0,0.0, 0.0,0.0,1.0));
    //Fram
    mVertices.push_back(Vertex(0.0,0.0,1.0, 1.0,0.0,1.0));
    mVertices.push_back(Vertex(1.0,0.0,1.0, 1.0,0.0,1.0));
    mVertices.push_back(Vertex(1.0,1.0,1.0, 1.0,0.0,1.0));
    mVertices.push_back(Vertex(1.0,1.0,1.0, 1.0,0.0,1.0));
    mVertices.push_back(Vertex(0.0,1.0,1.0, 1.0,0.0,1.0));
    mVertices.push_back(Vertex(0.0,0.0,1.0, 1.0,0.0,1.0));
    mMatrix.setToIdentity();
}

Cube::~Cube()
{

}

void Cube::draw()
{
    glBindVertexArray( mVAO );
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

void Cube::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

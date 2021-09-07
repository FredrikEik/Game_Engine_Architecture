#include "cube.h"
#include "Components.h"

Cube::Cube()
{
    // Front-1                 //Position           //RGB         //UV
    mVertices.push_back(Vertex{-0.1, -0.1,  0.1,     1,0,0,         0,0});
    mVertices.push_back(Vertex{ 0.1, -0.1,  0.1,     1,0,0,         1,0});
    mVertices.push_back(Vertex{ 0.1,  0.1,  0.1,     1,0,0,         1,1});
    // Front-2                    1     1     1
    mVertices.push_back(Vertex{ 0.1,  0.1,  0.1,     1,1,0,         0,0});
    mVertices.push_back(Vertex{-0.1,  0.1,  0.1,     1,1,0,         1,0});
    mVertices.push_back(Vertex{-0.1, -0.1,  0.1,     1,1,0,         1,1});
    // Back-1                     1     1     1
    mVertices.push_back(Vertex{ 0.1, -0.1, -0.1,     1,1,1,         0,0});
    mVertices.push_back(Vertex{-0.1, -0.1, -0.1,     1,1,1,         1,0});
    mVertices.push_back(Vertex{-0.1,  0.1, -0.1,     1,1,1,         1,1});
    // Back-2                     1     1     1
    mVertices.push_back(Vertex{-0.1,  0.1, -0.1,     0,1,1,         0,0});
    mVertices.push_back(Vertex{ 0.1,  0.1, -0.1,     0,1,1,         1,0});
    mVertices.push_back(Vertex{ 0.1, -0.1, -0.1,     0,1,1,         1,1});
    // Right-1                    1     1     1
    mVertices.push_back(Vertex{ 0.1, -0.1,  0.1,     1,0,1,         0,0});
    mVertices.push_back(Vertex{ 0.1, -0.1, -0.1,     1,0,1,         1,0});
    mVertices.push_back(Vertex{ 0.1,  0.1, -0.1,     1,0,1,         1,1});
    // Right-2                    1     1     1
    mVertices.push_back(Vertex{ 0.1,  0.1, -0.1,     1,0.5,0,       0,0});
    mVertices.push_back(Vertex{ 0.1,  0.1,  0.1,     1,0.5,0,       1,0});
    mVertices.push_back(Vertex{ 0.1, -0.1,  0.1,     1,0.5,0,       1,1});
    // Left-1                     1     1     1
    mVertices.push_back(Vertex{-0.1, -0.1, -0.1,     1,0.5,1,       0,0});
    mVertices.push_back(Vertex{-0.1, -0.1,  0.1,     1,0.5,1,       1,0});
    mVertices.push_back(Vertex{-0.1,  0.1,  0.1,     1,0.5,1,       1,1});
    // Left-2                     1     1     1
    mVertices.push_back(Vertex{-0.1,  0.1,  0.1,     0.5,0.5,0,     0,0});
    mVertices.push_back(Vertex{-0.1,  0.1, -0.1,     0.5,0.5,0,     1,0});
    mVertices.push_back(Vertex{-0.1, -0.1, -0.1,     0.5,0.5,0,     1,1});
    // Bot-1                      1     1     1
    mVertices.push_back(Vertex{-0.1, -0.1,  0.1,     0.5,0.5,0.5,   0,0});
    mVertices.push_back(Vertex{ 0.1, -0.1,  0.1,     0.5,0.5,0.5,   1,0});
    mVertices.push_back(Vertex{ 0.1, -0.1, -0.1,     0.5,0.5,0.5,   1,1});
    // Bot-2                      1     1     1
    mVertices.push_back(Vertex{ 0.1, -0.1, -0.1,     0.5,0.5,1,     0,0});
    mVertices.push_back(Vertex{-0.1, -0.1, -0.1,     0.5,0.5,1,     1,0});
    mVertices.push_back(Vertex{-0.1, -0.1,  0.1,     0.5,0.5,1,     1,1});
    // Top-1                      1     1     1
    mVertices.push_back(Vertex{-0.1,  0.1,  0.1,     0.5,0,0,       0,0});
    mVertices.push_back(Vertex{ 0.1,  0.1,  0.1,     0.5,0,0,       1,0});
    mVertices.push_back(Vertex{ 0.1,  0.1, -0.1,     0.5,0,0,       1,1});
    // Top-2                      1     1     1
    mVertices.push_back(Vertex{ 0.1,  0.1, -0.1,     0,0.5,0,       0,0});
    mVertices.push_back(Vertex{-0.1,  0.1, -0.1,     0,0.5,0,       1,0});
    mVertices.push_back(Vertex{-0.1,  0.1,  0.1,     0,0.5,0,       1,1});
}

void Cube::init()
{
    initializeOpenGLFunctions();

        glGenVertexArrays( 1, &mesh->mVAO );
        glBindVertexArray( mesh->mVAO );

        glGenBuffers( 1, &mesh->mVBO );
        glBindBuffer( GL_ARRAY_BUFFER, mesh->mVBO );

        glBufferData( GL_ARRAY_BUFFER,      //what buffer type
                      mVertices.size() * sizeof( Vertex ),   //how big buffer do we need
                      mVertices.data(),             //the actual vertices
                      GL_STATIC_DRAW        //should the buffer be updated on the GPU
                      );


        glBindBuffer(GL_ARRAY_BUFFER, mesh->mVBO);
        glVertexAttribPointer(
                    0,                  // attribute. No particular reason for 0, but must match layout(location = 0) in the vertex shader.
                    3,                  // size
                    GL_FLOAT,           // data type
                    GL_FALSE,           // normalized?
                    sizeof(Vertex),  // stride
                    reinterpret_cast<GLvoid*>(0));          // array buffer offset
        glEnableVertexAttribArray(0);


        glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
        glEnableVertexAttribArray(2);
        glBindVertexArray(0);
}

void Cube::draw()
{
    glBindVertexArray(mesh->mVAO);
    glUniformMatrix4fv(mesh->mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

#include "cube.h"

Cube::Cube()
{
    mesh = new Mesh();
    material = new Material();
    transform = new Transform();

    // Front-1                 //Position           //RGB         //UV
    mesh->mVertices.push_back(Vertex{-0.1, -0.1,  0.1,     1,0,0,         0,0});
    mesh->mVertices.push_back(Vertex{ 0.1, -0.1,  0.1,     1,0,0,         1,0});
    mesh->mVertices.push_back(Vertex{ 0.1,  0.1,  0.1,     1,0,0,         1,1});
    // Front-2                    1     1     1
    mesh->mVertices.push_back(Vertex{ 0.1,  0.1,  0.1,     1,1,0,         0,0});
    mesh->mVertices.push_back(Vertex{-0.1,  0.1,  0.1,     1,1,0,         1,0});
    mesh->mVertices.push_back(Vertex{-0.1, -0.1,  0.1,     1,1,0,         1,1});
    // Back-1                     1     1     1
    mesh->mVertices.push_back(Vertex{ 0.1, -0.1, -0.1,     1,1,1,         0,0});
    mesh->mVertices.push_back(Vertex{-0.1, -0.1, -0.1,     1,1,1,         1,0});
    mesh->mVertices.push_back(Vertex{-0.1,  0.1, -0.1,     1,1,1,         1,1});
    // Back-2                     1     1     1
    mesh->mVertices.push_back(Vertex{-0.1,  0.1, -0.1,     0,1,1,         0,0});
    mesh->mVertices.push_back(Vertex{ 0.1,  0.1, -0.1,     0,1,1,         1,0});
    mesh->mVertices.push_back(Vertex{ 0.1, -0.1, -0.1,     0,1,1,         1,1});
    // Right-1                    1     1     1
    mesh->mVertices.push_back(Vertex{ 0.1, -0.1,  0.1,     1,0,1,         0,0});
    mesh->mVertices.push_back(Vertex{ 0.1, -0.1, -0.1,     1,0,1,         1,0});
    mesh->mVertices.push_back(Vertex{ 0.1,  0.1, -0.1,     1,0,1,         1,1});
    // Right-2                    1     1     1
    mesh->mVertices.push_back(Vertex{ 0.1,  0.1, -0.1,     1,0.5,0,       0,0});
    mesh->mVertices.push_back(Vertex{ 0.1,  0.1,  0.1,     1,0.5,0,       1,0});
    mesh->mVertices.push_back(Vertex{ 0.1, -0.1,  0.1,     1,0.5,0,       1,1});
    // Left-1                     1     1     1
    mesh->mVertices.push_back(Vertex{-0.1, -0.1, -0.1,     1,0.5,1,       0,0});
    mesh->mVertices.push_back(Vertex{-0.1, -0.1,  0.1,     1,0.5,1,       1,0});
    mesh->mVertices.push_back(Vertex{-0.1,  0.1,  0.1,     1,0.5,1,       1,1});
    // Left-2                     1     1     1
    mesh->mVertices.push_back(Vertex{-0.1,  0.1,  0.1,     0.5,0.5,0,     0,0});
    mesh->mVertices.push_back(Vertex{-0.1,  0.1, -0.1,     0.5,0.5,0,     1,0});
    mesh->mVertices.push_back(Vertex{-0.1, -0.1, -0.1,     0.5,0.5,0,     1,1});
    // Bot-1                      1     1     1
    mesh->mVertices.push_back(Vertex{-0.1, -0.1,  0.1,     0.5,0.5,0.5,   0,0});
    mesh->mVertices.push_back(Vertex{ 0.1, -0.1,  0.1,     0.5,0.5,0.5,   1,0});
    mesh->mVertices.push_back(Vertex{ 0.1, -0.1, -0.1,     0.5,0.5,0.5,   1,1});
    // Bot-2                      1     1     1
    mesh->mVertices.push_back(Vertex{ 0.1, -0.1, -0.1,     0.5,0.5,1,     0,0});
    mesh->mVertices.push_back(Vertex{-0.1, -0.1, -0.1,     0.5,0.5,1,     1,0});
    mesh->mVertices.push_back(Vertex{-0.1, -0.1,  0.1,     0.5,0.5,1,     1,1});
    // Top-1                      1     1     1
    mesh->mVertices.push_back(Vertex{-0.1,  0.1,  0.1,     0.5,0,0,       0,0});
    mesh->mVertices.push_back(Vertex{ 0.1,  0.1,  0.1,     0.5,0,0,       1,0});
    mesh->mVertices.push_back(Vertex{ 0.1,  0.1, -0.1,     0.5,0,0,       1,1});
    // Top-2                      1     1     1
    mesh->mVertices.push_back(Vertex{ 0.1,  0.1, -0.1,     0,0.5,0,       0,0});
    mesh->mVertices.push_back(Vertex{-0.1,  0.1, -0.1,     0,0.5,0,       1,0});
    mesh->mVertices.push_back(Vertex{-0.1,  0.1,  0.1,     0,0.5,0,       1,1});

    transform->mMatrix.setToIdentity();
}

void Cube::init()
{
    initializeOpenGLFunctions();

    glGenVertexArrays( 1, &mesh->mVAO );
    glBindVertexArray( mesh->mVAO );

    glGenBuffers( 1, &mesh->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mesh->mVBO );

    glBufferData( GL_ARRAY_BUFFER,      //what buffer type
                  mesh->mVertices.size() * sizeof( Vertex ),   //how big buffer do we need
                  mesh->mVertices.data(),             //the actual vertices
                  GL_STATIC_DRAW        //should the buffer be updated on the GPU
                  );

    glBindBuffer(GL_ARRAY_BUFFER, mesh->mVBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Cube::draw()
{
    glBindVertexArray(mesh->mVAO);
    glDrawArrays(GL_TRIANGLES, 0, mesh->mVertices.size());
}

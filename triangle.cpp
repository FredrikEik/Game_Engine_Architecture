#include "triangle.h"
#include "components.h"

Triangle::Triangle()
{
    mMesh->mVertices.push_back(Vertex{-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  0.f, 0.f}); // Bottom Left
    mMesh->mVertices.push_back(Vertex{0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.f}); // Bottom Right
    mMesh->mVertices.push_back(Vertex{0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.f}); // Top
    mTransform->mMatrix.setToIdentity();
}

Triangle::~Triangle()
{
}

void Triangle::init()
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    glGenVertexArrays( 1, &mMesh->mVAO[1] );
    glBindVertexArray( mMesh->mVAO[1] );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mMesh->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mMesh->mVBO );

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, mMesh->mVertices.size()*sizeof( Vertex ), mMesh->mVertices.data(), GL_STATIC_DRAW );

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
    glBindVertexArray( mMesh->mVAO[1] );
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

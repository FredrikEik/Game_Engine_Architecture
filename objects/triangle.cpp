#include "triangle.h"
#include "vertex.h"
#include "systems/ecs/Components.h"


Triangle::Triangle()
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    tempMesh = new MeshComponent();

                            // Positions            // Colors       //UV
    tempMesh->mVertices.push_back(Vertex(-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  0.f, 0.f)); // Bottom Left
    tempMesh->mVertices.push_back(Vertex(0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.f)); // Bottom Right
    tempMesh-> mVertices.push_back(Vertex(0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.f)); // Top

    tempMesh->mDrawType = GL_TRIANGLES;

    mMaterial = new MaterialComponent();

}

Triangle::~Triangle()
{
}

void Triangle::init()
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    glGenVertexArrays( 1, &tempMesh->mVAO );
    glBindVertexArray( tempMesh->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &tempMesh->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, tempMesh->mVBO );

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, tempMesh->mVertices.size()*sizeof( Vertex ), tempMesh->mVertices.data(), GL_STATIC_DRAW );

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

//void Triangle::draw()
//{

//    glBindVertexArray( tempMesh->mVAO );
//    glDrawArrays(GL_TRIANGLES, 0, tempMesh->mVertices.size());
//    glBindVertexArray(0);
//}

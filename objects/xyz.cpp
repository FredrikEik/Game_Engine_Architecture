#include "xyz.h"
#include "vertex.h"
#include "shader.h"
#include "systems/ecs/Components.h"

XYZ::XYZ() {
mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();

tempMesh = new MeshComponent();

    tempMesh->mVertices.push_back(Vertex{0.f, 0.f, 0.f, 1.f, 0.f, 0.f});
    tempMesh->mVertices.push_back(Vertex{100.f, 0.f, 0.f, 1.f, 0.f, 0.f});
    tempMesh->mVertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 1.f, 0.f});
    tempMesh->mVertices.push_back(Vertex{0.f, 100.f, 0.f, 0.f, 1.f, 0.f});
    tempMesh->mVertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 0.f, 1.f});
    tempMesh->mVertices.push_back(Vertex{0.f, 0.f, 100.f, 0.f, 0.f, 1.f});

    tempMesh->mDrawType = GL_LINES;

    mMaterial = new MaterialComponent();
}

XYZ::~XYZ()
{
}

void XYZ::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &tempMesh->mVAO );
    glBindVertexArray( tempMesh->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &tempMesh->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, tempMesh->mVBO );

    glBufferData( GL_ARRAY_BUFFER, tempMesh->mVertices.size()*sizeof( Vertex ), tempMesh->mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, tempMesh->mVBO);
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

//void XYZ::draw()
//{
//    glBindVertexArray( tempMesh->mVAO );
//    glDrawArrays(GL_LINES, 0, tempMesh->mVertices.size());
//}

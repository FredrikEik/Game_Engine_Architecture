#include "xyz.h"
#include "vertex.h"
#include "shader.h"

XYZ::XYZ() {
    mTransformComp = new TransformComponent();
    mTransformComp->mMatrix.setToIdentity();
    mMeshComp = new MeshComponent();

    mMeshComp->mVertices.push_back(Vertex{0.f, 0.f, 0.f, 1.f, 0.f, 0.f});
    mMeshComp->mVertices.push_back(Vertex{100.f, 0.f, 0.f, 1.f, 0.f, 0.f});
    mMeshComp->mVertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 1.f, 0.f});
    mMeshComp->mVertices.push_back(Vertex{0.f, 100.f, 0.f, 0.f, 1.f, 0.f});
    mMeshComp->mVertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 0.f, 1.f});
    mMeshComp->mVertices.push_back(Vertex{0.f, 0.f, 100.f, 0.f, 0.f, 1.f});
    mTransformComp->mMatrix.setToIdentity();
    mMaterialComp = new MaterialComponent();
    mMeshComp->mDrawType = GL_LINES;

}

XYZ::~XYZ()
{
}

void XYZ::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mMeshComp->mVAO );
    glBindVertexArray( mMeshComp->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mMeshComp->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mMeshComp->mVBO );

    glBufferData( GL_ARRAY_BUFFER, mMeshComp->mVertices.size()*sizeof( Vertex ), mMeshComp->mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mMeshComp->mVBO);
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

void XYZ::draw()
{
    glBindVertexArray( mMeshComp->mVAO );
    glDrawArrays(GL_LINES, 0, mMeshComp->mVertices.size());
}

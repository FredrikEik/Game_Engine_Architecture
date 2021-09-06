#include "cube.h"
#include "vertex.h"
#include "systems/ecs/Components.h"

Cube::Cube()
{
    mComponents.push_back(new TransformComponent());
    TransformComponent *tempTrans = static_cast<TransformComponent*>(mComponents.back());
    tempTrans->mMatrix.setToIdentity();


    mComponents.push_back(new MeshComponent());
       MeshComponent *tempMesh = static_cast<MeshComponent*>(mComponents.back());

       tempMesh->mVertices.push_back(Vertex(-0.5,-0.5,0.5   , 1, 0, 1));
       tempMesh->mVertices.push_back(Vertex(0.5,-0.5,0.5    , 1, 0, 1));
       tempMesh->mVertices.push_back(Vertex(0.5,0.5,0.5     , 1, 0, 1));

       tempMesh->mVertices.push_back(Vertex(-0.5,-0.5,0.5   , 1, 0, 1));
       tempMesh->mVertices.push_back(Vertex(0.5,0.5,0.5     , 1, 0, 1));
       tempMesh->mVertices.push_back(Vertex(-0.5,0.5,0.5    , 1, 0, 1));

       tempMesh->mVertices.push_back(Vertex(-0.5,-0.5,-0.5  , 1, 0, 0));
       tempMesh->mVertices.push_back(Vertex(-0.5,-0.5,0.5   , 1, 0, 0));
       tempMesh->mVertices.push_back(Vertex(-0.5,0.5,0.5    , 1, 0, 0));

       tempMesh->mVertices.push_back(Vertex(-0.5,-0.5,-0.5  , 1, 0, 0));
       tempMesh->mVertices.push_back(Vertex(-0.5,0.5,0.5    , 1, 0, 0));
       tempMesh->mVertices.push_back(Vertex(-0.5,0.5,-0.5   , 1, 0, 0));

       tempMesh->mVertices.push_back(Vertex(0.5,0.5,-0.5    , 0, 1, 0));
       tempMesh->mVertices.push_back(Vertex(-0.5,0.5,-0.5   , 0, 1, 0));
       tempMesh->mVertices.push_back(Vertex(-0.5,-0.5,-0.5  , 0, 1, 0));

       tempMesh->mVertices.push_back(Vertex(0.5,0.5,-0.5    , 0, 1, 0));
       tempMesh->mVertices.push_back(Vertex(-0.5,-0.5,-0.5  , 0, 1, 0));
       tempMesh->mVertices.push_back(Vertex(0.5,-0.5,-0.5   , 0, 1, 0));

       tempMesh->mVertices.push_back(Vertex(0.5,0.5,0.5     , 1, 0, 1));
       tempMesh->mVertices.push_back(Vertex(0.5,0.5,-0.5    , 1, 0, 1));
       tempMesh->mVertices.push_back(Vertex(0.5,-0.5,-0.5   , 1, 0, 1));

       tempMesh->mVertices.push_back(Vertex(0.5,-0.5,0.5    , 1, 0, 1));
       tempMesh->mVertices.push_back(Vertex(0.5,-0.5,-0.5   , 1, 0, 1));
       tempMesh->mVertices.push_back(Vertex(0.5,0.5,0.5     , 1, 0, 1));

       tempMesh->mVertices.push_back(Vertex(-0.5,-0.5,0.5   , 1, 1, 0));
       tempMesh->mVertices.push_back(Vertex(-0.5,-0.5,-0.5  , 1, 1, 0));
       tempMesh->mVertices.push_back(Vertex(0.5,-0.5,-0.5   , 1, 1, 0));

       tempMesh->mVertices.push_back(Vertex(0.5,-0.5,0.5    , 1, 1, 0));
       tempMesh->mVertices.push_back(Vertex(-0.5,-0.5,0.5   , 1, 1, 0));
       tempMesh->mVertices.push_back(Vertex(0.5,-0.5,-0.5   , 1, 1, 0));

       tempMesh->mVertices.push_back(Vertex(-0.5,0.5,0.5    , 1, 0, 0));
       tempMesh->mVertices.push_back(Vertex(-0.5,0.5,-0.5   , 1, 0, 0));
       tempMesh->mVertices.push_back(Vertex(0.5,0.5,-0.5    , 1, 0, 0));

       tempMesh->mVertices.push_back(Vertex(0.5,0.5,0.5     , 1, 0, 0));
       tempMesh->mVertices.push_back(Vertex(-0.5,0.5,0.5    , 1, 0, 0));
       tempMesh->mVertices.push_back(Vertex(0.5,0.5,-0.5    , 1, 0, 0));



}

Cube::~Cube()
{
}

void Cube::init()
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    MeshComponent *tempMesh = static_cast<MeshComponent*>(mComponents.at(1));

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

void Cube::draw()
{
    MeshComponent *tempMesh = static_cast<MeshComponent*>(mComponents.at(1));
    glBindVertexArray( tempMesh->mVAO );
    glDrawArrays(GL_TRIANGLES, 0, tempMesh->mVertices.size());
    glBindVertexArray(0);

    //qDebug() << "draw cube";
}

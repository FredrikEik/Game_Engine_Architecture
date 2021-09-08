#include "cube.h"
#include "components.h"

Cube::Cube()
{
    MeshComp = new MeshComponent();
    TransformComp = new TransformComponent();
    TextureComp = new TextureComponent();


    MeshComp->mVertices.push_back(Vertex(-0.5,-0.5,0.5   , 1, 0, 1));
    MeshComp->mVertices.push_back(Vertex(0.5,-0.5,0.5    , 1, 0, 1));
    MeshComp->mVertices.push_back(Vertex(0.5,0.5,0.5     , 1, 0, 1));

    MeshComp->mVertices.push_back(Vertex(-0.5,-0.5,0.5   , 1, 0, 1));
    MeshComp->mVertices.push_back(Vertex(0.5,0.5,0.5     , 1, 0, 1));
    MeshComp->mVertices.push_back(Vertex(-0.5,0.5,0.5    , 1, 0, 1));

    MeshComp->mVertices.push_back(Vertex(-0.5,-0.5,-0.5  , 1, 0, 0));
    MeshComp->mVertices.push_back(Vertex(-0.5,-0.5,0.5   , 1, 0, 0));
    MeshComp->mVertices.push_back(Vertex(-0.5,0.5,0.5    , 1, 0, 0));

    MeshComp->mVertices.push_back(Vertex(-0.5,-0.5,-0.5  , 1, 0, 0));
    MeshComp->mVertices.push_back(Vertex(-0.5,0.5,0.5    , 1, 0, 0));
    MeshComp->mVertices.push_back(Vertex(-0.5,0.5,-0.5   , 1, 0, 0));

    MeshComp->mVertices.push_back(Vertex(0.5,0.5,-0.5    , 0, 1, 0));
    MeshComp->mVertices.push_back(Vertex(-0.5,0.5,-0.5   , 0, 1, 0));
    MeshComp->mVertices.push_back(Vertex(-0.5,-0.5,-0.5  , 0, 1, 0));

    MeshComp->mVertices.push_back(Vertex(0.5,0.5,-0.5    , 0, 1, 0));
    MeshComp->mVertices.push_back(Vertex(-0.5,-0.5,-0.5  , 0, 1, 0));
    MeshComp->mVertices.push_back(Vertex(0.5,-0.5,-0.5   , 0, 1, 0));

    MeshComp->mVertices.push_back(Vertex(0.5,0.5,0.5     , 1, 0, 1));
    MeshComp->mVertices.push_back(Vertex(0.5,0.5,-0.5    , 1, 0, 1));
    MeshComp->mVertices.push_back(Vertex(0.5,-0.5,-0.5   , 1, 0, 1));

    MeshComp->mVertices.push_back(Vertex(0.5,-0.5,0.5    , 1, 0, 1));
    MeshComp->mVertices.push_back(Vertex(0.5,-0.5,-0.5   , 1, 0, 1));
    MeshComp->mVertices.push_back(Vertex(0.5,0.5,0.5     , 1, 0, 1));

    MeshComp->mVertices.push_back(Vertex(-0.5,-0.5,0.5   , 1, 1, 0));
    MeshComp->mVertices.push_back(Vertex(-0.5,-0.5,-0.5  , 1, 1, 0));
    MeshComp->mVertices.push_back(Vertex(0.5,-0.5,-0.5   , 1, 1, 0));

    MeshComp->mVertices.push_back(Vertex(0.5,-0.5,0.5    , 1, 1, 0));
    MeshComp->mVertices.push_back(Vertex(-0.5,-0.5,0.5   , 1, 1, 0));
    MeshComp->mVertices.push_back(Vertex(0.5,-0.5,-0.5   , 1, 1, 0));

    MeshComp->mVertices.push_back(Vertex(-0.5,0.5,0.5    , 1, 0, 0));
    MeshComp->mVertices.push_back(Vertex(-0.5,0.5,-0.5   , 1, 0, 0));
    MeshComp->mVertices.push_back(Vertex(0.5,0.5,-0.5    , 1, 0, 0));

    MeshComp->mVertices.push_back(Vertex(0.5,0.5,0.5     , 1, 0, 0));
    MeshComp->mVertices.push_back(Vertex(-0.5,0.5,0.5    , 1, 0, 0));
    MeshComp->mVertices.push_back(Vertex(0.5,0.5,-0.5    , 1, 0, 0));

    TransformComp->mMatrix.setToIdentity();

}

Cube::~Cube(){}

void Cube::init()
{

    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &MeshComp->mVAO );
    glBindVertexArray( MeshComp->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &MeshComp->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, MeshComp->mVBO );

    glBufferData( GL_ARRAY_BUFFER, MeshComp->mVertices.size()*sizeof( Vertex ), MeshComp->mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, MeshComp->mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE,  sizeof( Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Cube::draw()
{
    glBindVertexArray( MeshComp->mVAO );
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

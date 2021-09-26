#include "cube.h"
#include "vertex.h"
#include "components.h"

Cube::Cube()
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();

    mMesh = new MeshComponent();
    // Positions            // Colors       //UV

    //front
    mMesh->mVertices.push_back(Vertex{0.0f,0.0f,1.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{1.0f,0.0f,1.0f,   0.0f,0.0f,0.0f,   1.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{1.0f,1.0f,1.0f,   0.0f,0.0f,0.0f,   1.0f,1.0f});
    mMesh->mVertices.push_back(Vertex{1.0f,1.0f,1.0f,   0.0f,0.0f,0.0f,   1.0f,1.0f});
    mMesh->mVertices.push_back(Vertex{0.0f,1.0f,1.0f,   0.0f,0.0f,0.0f,   0.0f,1.0f});
    mMesh->mVertices.push_back(Vertex{0.0f,0.0f,1.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    //left
    mMesh->mVertices.push_back(Vertex{0.0f,0.0f,0.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{0.0f,0.0f,1.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{0.0f,1.0f,1.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{0.0f,1.0f,1.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{0.0f,1.0f,0.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{0.0f,0.0f,0.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    //bottom
    mMesh->mVertices.push_back(Vertex{0.0f,0.0f,0.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{1.0f,0.0f,0.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{1.0f,0.0f,1.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{1.0f,0.0f,1.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{0.0f,0.0f,1.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{0.0f,0.0f,0.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    //right
    mMesh->mVertices.push_back(Vertex{1.0f,0.0f,0.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{1.0f,0.0f,1.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{1.0f,1.0f,1.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{1.0f,1.0f,1.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{1.0f,1.0f,0.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{1.0f,0.0f,0.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    //back
    mMesh->mVertices.push_back(Vertex{0.0f,0.0f,0.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{1.0f,0.0f,0.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{1.0f,1.0f,0.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{1.0f,1.0f,0.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{0.0f,1.0f,0.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{0.0f,0.0f,0.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    //top
    mMesh->mVertices.push_back(Vertex{0.0f,1.0f,0.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{1.0f,1.0f,0.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{1.0f,1.0f,1.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{1.0f,1.0f,1.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{0.0f,1.0f,1.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});
    mMesh->mVertices.push_back(Vertex{0.0f,1.0f,0.0f,   0.0f,0.0f,0.0f,   0.0f,0.0f});

    mMesh->mDrawType = GL_TRIANGLES;

    mMaterial = new MaterialComponent();
}

Cube::~Cube()
{
}

void Cube::init()
{
    //must call this to use OpenGL functions?
    initializeOpenGLFunctions();


    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mMesh->mVAO );
    glBindVertexArray( mMesh->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mMesh->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mMesh->mVBO );

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, mMesh->mVertices.size()*sizeof( Vertex ),
                  mMesh->mVertices.data(), GL_STATIC_DRAW );

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

//void Cube::draw()
//{
//    MeshComponent *tempMesh = static_cast<MeshComponent*>(mComponents.at(1));
//    glBindVertexArray( tempMesh->mVAO );
//    glDrawArrays(GL_TRIANGLES, 0, tempMesh->mVertices.size());
//    glBindVertexArray(0);
//}

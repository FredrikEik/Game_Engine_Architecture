#include "light.h"
#include <QOpenGLFunctions_4_1_Core>
#include "renderwindow.h"

//using namespace gsls21;

Light::Light()
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMesh = new MeshComponent();
    mMaterial = new MaterialComponent();
    mCollision = new CollisionComponent;
    makeVerticies(mMesh);
    mNameComp = new NameComponent();
    mNameComp->mName = "Light";
}

Light::~Light()
{

}

void Light::makeVerticies(MeshComponent *dMesh)
{
    GL_TRIANGLES;

    dMesh->mVertices.insert(dMesh->mVertices.end(),
    {
     Vertex(gsl::Vector3D{-0.25f, -0.25f,  0.25f},    gsl::Vector3D{0.8f, 0.8f, 0.3f},  gsl::Vector2D{0.f,  0.f}),   //Left low
     Vertex(gsl::Vector3D( 0.25f, -0.25f,  0.25f),    gsl::Vector3D(0.8f, 0.8f, 0.3f),  gsl::Vector2D(1.f,  0.f)),   //Right low
     Vertex(gsl::Vector3D( 0.0f,  0.25f,  0.0f),      gsl::Vector3D(0.8f, 0.8f, 0.3f),  gsl::Vector2D(0.5f, 0.5f)),  //Top
     Vertex(gsl::Vector3D( 0.0f, -0.25f, -0.25f),     gsl::Vector3D(0.8f, 0.8f, 0.3f),  gsl::Vector2D(0.5f, 0.5f))   //Back low
     });



    dMesh->mIndices.insert( dMesh->mIndices.end(),
    { 0, 1, 2,
      1, 3, 2,
      3, 0, 2,
      0, 3, 1
    });

}

void Light::init()
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

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &mMesh->mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mMesh->mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mMesh->mIndices.size() * sizeof(GLuint), mMesh->mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

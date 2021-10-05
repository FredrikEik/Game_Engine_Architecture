//#include "xyz.h"
//#include "vertex.h"
//#include "shader.h"
//#include "components.h"

//XYZ::XYZ()
//{
//    mMesh->mVertices.push_back(Vertex{0.f, 0.f, 0.f, 1.f, 0.f, 0.f});
//    mMesh->mVertices.push_back(Vertex{100.f, 0.f, 0.f, 1.f, 0.f, 0.f});
//    mMesh->mVertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 1.f, 0.f});
//    mMesh->mVertices.push_back(Vertex{0.f, 100.f, 0.f, 0.f, 1.f, 0.f});
//    mMesh->mVertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 0.f, 1.f});
//    mMesh->mVertices.push_back(Vertex{0.f, 0.f, 100.f, 0.f, 0.f, 1.f});

////    getTransformComp()->mMatrix.setToIdentity();
//}

//XYZ::~XYZ()
//{
//}

//void XYZ::init()
//{
//    initializeOpenGLFunctions();

//    //Vertex Array Object - VAO
//    glGenVertexArrays( 1, &mMesh->mVAO[1] );
//    glBindVertexArray( mMesh->mVAO[1] );

//    //Vertex Buffer Object to hold vertices - VBO
//    glGenBuffers( 1, &mMesh->mVBO );
//    glBindBuffer( GL_ARRAY_BUFFER, mMesh->mVBO );

//    glBufferData( GL_ARRAY_BUFFER, mMesh->mVertices.size()*sizeof( Vertex ), mMesh->mVertices.data(), GL_STATIC_DRAW );

//    // 1rst attribute buffer : vertices
//    glBindBuffer(GL_ARRAY_BUFFER, mMesh->mVBO);
//    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
//    glEnableVertexAttribArray(0);

//    // 2nd attribute buffer : colors
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
//    glEnableVertexAttribArray(1);

//    // 3rd attribute buffer : uvs
//    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
//    glEnableVertexAttribArray(2);

//    glBindVertexArray(0);
//}

//void XYZ::draw()
//{
//    glBindVertexArray( mMesh->mVAO );
//    glDrawArrays(GL_LINES, 0, mMesh->mVertices.size());
//}

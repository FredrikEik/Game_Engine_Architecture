//#include "xyz.h"
//#include "vertex.h"
//#include "shader.h"


//XYZ::XYZ(){

//    mesh = new Mesh();
//    material = new Material();
//    transform = new Transform();

//    mesh->mVertices.push_back(Vertex{0.f, 0.f, 0.f, 1.f, 0.f, 0.f});
//    mesh->mVertices.push_back(Vertex{100.f, 0.f, 0.f, 1.f, 0.f, 0.f});
//    mesh->mVertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 1.f, 0.f});
//    mesh->mVertices.push_back(Vertex{0.f, 100.f, 0.f, 0.f, 1.f, 0.f});
//    mesh->mVertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 0.f, 1.f});
//    mesh->mVertices.push_back(Vertex{0.f, 0.f, 100.f, 0.f, 0.f, 1.f});

//    transform->mMatrix.setToIdentity();
//}

//void XYZ::init()
//{
//    initializeOpenGLFunctions();

//    //Vertex Array Object - VAO
//    glGenVertexArrays( 1, &mesh->mVAO );
//    glBindVertexArray( mesh->mVAO );

//    //Vertex Buffer Object to hold vertices - VBO
//    glGenBuffers( 1, &mesh->mVBO );
//    glBindBuffer( GL_ARRAY_BUFFER, mesh->mVBO );

//    glBufferData( GL_ARRAY_BUFFER, mesh->mVertices.size()*sizeof( Vertex ), mesh->mVertices.data(), GL_STATIC_DRAW );

//    // 1st attribute buffer : vertices
//    glBindBuffer(GL_ARRAY_BUFFER, mesh->mVBO);
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
//    glBindVertexArray( mesh->mVAO );
//    glDrawArrays(GL_LINES, 0, mesh->mVertices.size());
//}

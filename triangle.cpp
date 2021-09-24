//#include "triangle.h"
//#include "vertex.h"

//Triangle::Triangle()
//{
//    mesh = new Mesh();
//    material = new Material();
//    transform = new Transform();

//                                // Positions            // Colors       //UV
//    mesh->mVertices.push_back(Vertex{-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  0.f, 0.f}); // Bottom Left
//    mesh->mVertices.push_back(Vertex{0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.f}); // Bottom Right
//    mesh->mVertices.push_back(Vertex{0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.f}); // Top

//    transform->mMatrix.setToIdentity();
//}

//Triangle::~Triangle()
//{

//}

//void Triangle::init()
//{
//    //must call this to use OpenGL functions
//    initializeOpenGLFunctions();

//    glGenVertexArrays( 1, &mesh->mVAO );
//    glBindVertexArray( mesh->mVAO );

//    //Vertex Buffer Object to hold vertices - VBO
//    glGenBuffers( 1, &mesh->mVBO );
//    glBindBuffer( GL_ARRAY_BUFFER, mesh->mVBO );

//    //Vertex Buffer Object to hold vertices - VBO
//    glBufferData( GL_ARRAY_BUFFER, mesh->mVertices.size()*sizeof( Vertex ), mesh->mVertices.data(), GL_STATIC_DRAW );

//    // 1st attribute buffer : vertices
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0  );          // array buffer offset
//    glEnableVertexAttribArray(0);

//    // 2nd attribute buffer : colors
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
//    glEnableVertexAttribArray(1);

//    // 3rd attribute buffer : uvs
//    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
//    glEnableVertexAttribArray(2);

//    glBindVertexArray(0);
//}

//void Triangle::draw()
//{
//    glBindVertexArray( mesh->mVAO );
//    glDrawArrays(GL_TRIANGLES, 0, 3);
//    glBindVertexArray(0);
//}

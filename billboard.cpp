#include "billboard.h"

Billboard::Billboard()
{
    getMeshComponent()->mVertices.push_back(Vertex(-1, -1,0.f,0,0,0,0,0)); //bottom left
    getMeshComponent()->mVertices.push_back(Vertex(0.f, -1,0.f,0,0,0,1,0)); //bottom right
    getMeshComponent()->mVertices.push_back(Vertex(-1.f, -0.8f,0.f,0,0,0,0,1)); // top left


    getMeshComponent()->mVertices.push_back(Vertex(0.f, -0.8f,0.f,0,0,0,1,1));  //top right
    getMeshComponent()->mVertices.push_back(Vertex(0.f, -1,0.f,0,0,0,1,0)); // bottom right
    getMeshComponent()->mVertices.push_back(Vertex(-1.f, -0.8f,0.f,0,0,0,0,1)); // top left

}

Billboard::~Billboard()
{

}

void Billboard::init()
{
    initializeOpenGLFunctions();

       // Set what shader you want to use to render this object
       //mMaterial->setActiveShader(ShaderType::TEXTURE_SHADER);
       //mMaterial->setActiveTextureSlot(2);
       //mMaterial->setupModelMatrixUniform(mMatrixUniform, matrixUniform);

       glGenVertexArrays( 1, &getMeshComponent()->mVAO );
       glBindVertexArray( getMeshComponent()->mVAO );


       glGenBuffers( 1, &getMeshComponent()->mVBO );
       glBindBuffer( GL_ARRAY_BUFFER, getMeshComponent()->mVBO );

       glBufferData( GL_ARRAY_BUFFER,                     //what buffer type
                     getMeshComponent()->mVertices.size() * sizeof( Vertex ), //how big buffer do we need
                     getMeshComponent()->mVertices.data(),                    //the actual vertices
                     GL_STATIC_DRAW                       //should the buffer be updated on the GPU
                     );

       glBindBuffer(GL_ARRAY_BUFFER, getMeshComponent()->mVBO);

       // 1rst attribute buffer : coordinates

       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(0));
       glEnableVertexAttribArray(0);

       // 2nd attribute buffer : colors
       glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)) );
       glEnableVertexAttribArray(1);

       // 3rd attribute buffer : uvs
       glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
       glEnableVertexAttribArray(2);

       glBindVertexArray(0);
}


void Billboard::draw()
{
    glBindVertexArray( getMeshComponent()->mVAO );
    glDrawArrays(getMeshComponent()->mDrawType, 0, getMeshComponent()->mVertices.size());
    glBindVertexArray(0);
}

void Billboard::move(float x, float y, float z)
{
    getTransformComponent()->mMatrix.translate(x,y,z);

}

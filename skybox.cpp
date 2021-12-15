#include "skybox.h"

/**
 * @brief SkyBox::SkyBox
 */
SkyBox::SkyBox()
{
    getTransformComp()->mTrueScaleMatrix = getTransformComp()->mMatrix;
}

SkyBox::~SkyBox()
{

}

/**
 * @brief SkyBox::Update
 * @param cameraPos
 */
void SkyBox::Update(gsl::Vector3D cameraPos)
{
    getTransformComp()->mMatrix.setPosition(cameraPos.x, cameraPos.y, cameraPos.z);
    getTransformComp()->mTrueScaleMatrix.setPosition(cameraPos.x, cameraPos.y, cameraPos.z);
}

void SkyBox::init()
{
    initializeOpenGLFunctions();

       glGenVertexArrays( 1, &getMeshComp()->mVAO );
       glBindVertexArray( getMeshComp()->mVAO );


       glGenBuffers( 1, &getMeshComp()->mVBO );
       glBindBuffer( GL_ARRAY_BUFFER, getMeshComp()->mVBO );

       glBufferData( GL_ARRAY_BUFFER,                     //what buffer type
                     getMeshComp()->mVertices.size() * sizeof( Vertex ), //how big buffer do we need
                     getMeshComp()->mVertices.data(),                    //the actual vertices
                     GL_STATIC_DRAW                       //should the buffer be updated on the GPU
                     );

       glBindBuffer(GL_ARRAY_BUFFER, getMeshComp()->mVBO);

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


void SkyBox::draw()
{
    glBindVertexArray( getMeshComp()->mVAO );
    //glUniformMatrix4fv( MeshComp->mMatrixUniform, 1, GL_FALSE, transformComp->mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, getMeshComp()->mVertices.size());
    glBindVertexArray(0);
}

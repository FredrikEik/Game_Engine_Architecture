#include "rendersystem.h"


RenderSystem::RenderSystem()
{

}

void RenderSystem::init(MeshComponent *mMesh)
{

    //must call this to use OpenGL functions?
    initializeOpenGLFunctions();


    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mMesh->mVAO[0] );
    glBindVertexArray( mMesh->mVAO[0] );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mMesh->mVBO[0] );
    glBindBuffer( GL_ARRAY_BUFFER, mMesh->mVBO[0] );

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

void RenderSystem::init(std::vector<Vertex> * vertexData, GLuint * VAO, GLuint * VBO)
{
        //must call this to use OpenGL functions?
        initializeOpenGLFunctions();


        //qDebug() << "Initialized: VAO Index" <<  VAO;
        //Vertex Array Object - VAO
        glGenVertexArrays( 1, VAO);
        glBindVertexArray( *VAO );
        //qDebug() << "Initialized: VAO Index" <<  VAO;
        //Vertex Buffer Object to hold vertices - VBO
        glGenBuffers( 1, VBO );
        glBindBuffer( GL_ARRAY_BUFFER, *VBO );

        //Vertex Buffer Object to hold vertices - VBO
        glBufferData( GL_ARRAY_BUFFER, vertexData->size()*sizeof( Vertex ),
                      vertexData->data(), GL_STATIC_DRAW );

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


void RenderSystem::draw(MeshComponent* mMesh)
{
    glBindVertexArray( mMesh->mVAO[0] );
    glDrawArrays(mMesh->mDrawType, 0, mMesh->VertexSize[0]);
    glBindVertexArray(0);
}


void RenderSystem::draw(MeshComponent *mMesh, MaterialComponent *mMaterial,TransformComponent* mTrasform, GLint viewMatrix, GLint projectionMatrix, GLint modelMatrix,Camera* camera )
{
    if(mMesh->isDrawable && mMesh->isDead ==false)
    {
        initializeOpenGLFunctions();    //must call this every frame it seems...

       glUniformMatrix4fv( viewMatrix, 1, GL_TRUE, camera->Cam.mViewMatrix.constData());
       glUniformMatrix4fv( projectionMatrix, 1, GL_TRUE, camera->Cam.mProjectionMatrix.constData());
       glUniformMatrix4fv( modelMatrix, 1, GL_TRUE,mTrasform->mMatrix.constData());
       //draw the object
       glBindVertexArray( mMesh->mVAO[mMesh->LODLevel] );

       if(mMesh->IsRay)
       {
           glDrawArrays(mMesh->mDrawType, 0, mMesh->mVertices.size());
       }
       else
       {
            glDrawArrays(mMesh->mDrawType, 0, mMesh->VertexSize[mMesh->LODLevel]);
       }
       glBindVertexArray(0);
    }


}

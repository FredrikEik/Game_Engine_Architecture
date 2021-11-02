#include "rendersystem.h"


RenderSystem::RenderSystem()
{

}

void RenderSystem::init(MeshComponent *mMesh)
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






/*
    //must call this to use OpenGL functions?
    initializeOpenGLFunctions();


    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mMesh->mVAO );
    glBindVertexArray( mMesh->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mMesh->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mMesh->mVBO );

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, mMesh->mRVertices->size()*sizeof( Vertex ),
                  mMesh->mRVertices->data(), GL_STATIC_DRAW );

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
    */
}

void RenderSystem::init(std::vector<Vertex> * vertexData, GLuint * VAO, GLuint * VBO)
{
        //must call this to use OpenGL functions?
        initializeOpenGLFunctions();


        //qDebug() << "Initialized: VAO Index" <<  VAO;
        //Vertex Array Object - VAO
        glGenVertexArrays( 1, VAO);
        glBindVertexArray( *VAO );
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
/*
void RenderSystem::initData(meshData * obj)
{
        //must call this to use OpenGL functions?
        //initializeOpenGLFunctions();


        //Vertex Array Object - VAO
        glGenVertexArrays( 1, &obj->VAO);
        glBindVertexArray( obj->VAO );

        //Vertex Buffer Object to hold vertices - VBO
        glGenBuffers( 1, &obj->VBO );
        glBindBuffer( GL_ARRAY_BUFFER, obj->VBO );

        //Vertex Buffer Object to hold vertices -5 VBO
        glBufferData( GL_ARRAY_BUFFER, obj->meshVert.size()*sizeof( Vertex ),
                      obj->meshVert.data(), GL_STATIC_DRAW );

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
}*/

void RenderSystem::draw(MeshComponent* mMesh)
{
    glBindVertexArray( mMesh->mVAO );
    glDrawArrays(GL_TRIANGLES, 0, mMesh->mVertices.size());
    glBindVertexArray(0);
}


void RenderSystem::draw(MeshComponent *mMesh, MaterialComponent *mMaterial,TransformComponent* mTrasform, GLint viewMatrix, GLint projectionMatrix, GLint modelMatrix,Camera* camera )
{
    if(mMesh->isDrawable)
    {
        initializeOpenGLFunctions();    //must call this every frame it seems...

       glUniformMatrix4fv( viewMatrix, 1, GL_TRUE, camera->Cam.mViewMatrix.constData());
       glUniformMatrix4fv( projectionMatrix, 1, GL_TRUE, camera->Cam.mProjectionMatrix.constData());
       glUniformMatrix4fv( modelMatrix, 1, GL_TRUE,mTrasform->mMatrix.constData());
       //draw the object
       glBindVertexArray( mMesh->mVAO );
       glDrawArrays(mMesh->mDrawType, 0, mMesh->mVertices.size());
       glBindVertexArray(0);
    }

/*
    initializeOpenGLFunctions();    //must call this every frame it seems...

   int viewMatrix1{-1};
   int projectionMatrix1{-1};
   int modelMatrix1{-1};

   viewMatrix1 = viewMatrix;
   projectionMatrix1 = projectionMatrix;
   modelMatrix1 = modelMatrix;

   //Now mMaterial component holds texture slot directly - probably should be changed


   glUniformMatrix4fv( viewMatrix1, 1, GL_TRUE, camera->Cam.mViewMatrix.constData());
   glUniformMatrix4fv( projectionMatrix1, 1, GL_TRUE, camera->Cam.mProjectionMatrix.constData());
   glUniformMatrix4fv( modelMatrix1, 1, GL_TRUE,mTrasform->mMatrix.constData());

   //draw the object
   glBindVertexArray( mMesh->mVAO );
   glDrawArrays(GL_TRIANGLES, 0, mMesh->mRVertices->size());
   glBindVertexArray(0);
   */
}

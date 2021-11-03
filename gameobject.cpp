#include "gameobject.h"

GameObject::GameObject()
{
    transformComp.mMatrix.setToIdentity();
}

GameObject::~GameObject()
{
   //TODO: do this in the proper component
   //glDeleteVertexArrays( 1, &meshComp->mVAO );
   //glDeleteBuffers( 1, &meshComp->mVBO );
}

void GameObject::draw()
{
    glBindVertexArray( meshComp.mVAO );
    if(meshComp.mIndexCount > 0)
    {
        glDrawElements(meshComp.mDrawType, meshComp.mIndexCount, GL_UNSIGNED_INT, nullptr);
    }
    else
    {
        glDrawArrays(meshComp.mDrawType, 0, meshComp.mVertexCount);
    }
    glBindVertexArray(0);
}

/*
void GameObject::initMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices)
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
                  vertices.size() * sizeof( Vertex ), //how big buffer do we need
                  vertices.data(),                    //the actual vertices
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

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &getMeshComponent()->mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getMeshComponent()->mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void GameObject::initMesh(std::vector<Vertex> vertices)
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
                  vertices.size() * sizeof( Vertex ), //how big buffer do we need
                  vertices.data(),                    //the actual vertices
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
*/
void GameObject::move(float x, float y, float z)
{
    transformComp.mMatrix.translate(x,y,z);
    sphereCollisionComp.center += gsl::Vector3D(x,y,z);
}

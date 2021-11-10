#include "player.h"


Player::Player()
{

    cameraTarget = getTransformComponent()->mMatrix.getPosition() + cameraOffset;        //create cameraTarget
}

Player::~Player()
{

}


void Player::init()
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

       //Second buffer - holds the indices (Element Array Buffer - EAB):
       glGenBuffers(1, &getMeshComponent()->mEAB);
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getMeshComponent()->mEAB);
       glBufferData(GL_ELEMENT_ARRAY_BUFFER, getMeshComponent()->mIndices.size() * sizeof(GLuint), getMeshComponent()->mIndices.data(), GL_STATIC_DRAW);

       glBindVertexArray(0);
}


void Player::move(float x, float y, float z)        //move the player
{
    getTransformComponent()->mMatrix.translate(x,y,z);
    getSphereCollisionComponent()->center += gsl::Vector3D(x,y,z);

    cameraTarget = getTransformComponent()->mMatrix.getPosition() + cameraOffset;
}

void Player::movement()     //set up movement
{
    if(input.W)
        move(movementSpeed,0,0);
    if(input.S)
        move(-movementSpeed,0,0);
    if(input.D)
        move(0,0,movementSpeed);
    if(input.A)
        move(0,0,-movementSpeed);

}
void Player::draw()
{
    glBindVertexArray( getMeshComponent()->mVAO );
    //glDrawArrays(GL_TRIANGLES, 0, getMeshComponent()->mVertices.size());
    glDrawElements(GL_TRIANGLES, getMeshComponent()->mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}





#include "player.h"
#include "lassurface.h"

Player::Player()
{

    cameraTarget = getTransformComponent()->mMatrix.getPosition() + cameraOffset;
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


void Player::move(float x, float y, float z)
{
    getTransformComponent()->mMatrix.translate(x,y,z);
    getSphereCollisionComponent()->center += gsl::Vector3D(x,y,z);

    cameraTarget = getTransformComponent()->mMatrix.getPosition() + cameraOffset;
}

void Player::movement()
{
    if(input.W)
        move(movementSpeed,0,0);
    if(input.S)
        move(-movementSpeed,0,0);
    if(input.D)
        move(0,0,movementSpeed);
    if(input.A)
        move(0,0,-movementSpeed);

    if(surfaceToWalkOn)
    {
        std::vector<Vertex>& vertices = dynamic_cast<class LASsurface*>(surfaceToWalkOn)->getMeshComponent()->mVertices;
        std::vector<GLuint>& indices  = dynamic_cast<class LASsurface*>(surfaceToWalkOn)->getMeshComponent()->mIndices;
        gsl::Vector3D barycCoords;
        gsl::Vector3D playerPos = getTransformComponent()->mMatrix.getPosition();

        for(int i = 0; i < indices.size() - 2; i+= 3)
        {
            gsl::Vector3D p1, p2, p3;
            p1 = gsl::Vector3D(vertices[indices[i]].getXYZ());
            p2 = gsl::Vector3D(vertices[indices[i+1]].getXYZ());
            p3 = gsl::Vector3D(vertices[indices[i+2]].getXYZ());

            barycCoords = playerPos.barycentricCoordinates(p1, p2, p3);
            //qDebug() << i << barycCoords.x << barycCoords.y << barycCoords.z;

            if(barycCoords.x >= 0 && barycCoords.y >= 0 && barycCoords.z >= 0)
            {
                qDebug() << "is inside triangle: " << i;
                float newY = p1.y*barycCoords.x + p2.y*barycCoords.y + p3.y*barycCoords.z;
                getTransformComponent()->mMatrix.setY(newY);
            }
        }
    }

}
void Player::draw()
{
    glBindVertexArray( getMeshComponent()->mVAO );
    //glDrawArrays(GL_TRIANGLES, 0, getMeshComponent()->mVertices.size());
    glDrawElements(GL_TRIANGLES, getMeshComponent()->mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}





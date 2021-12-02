#include "rollingball.h"

RollingBall::RollingBall(int n, GameObject* p) : OctahedronBall (n)
{
    force.z = -9.81f;
    plane = p;
    mTexture = 3;
}

RollingBall::~RollingBall()
{

}

void RollingBall::move(float dt)
{
    std::vector<Vertex>& vertices = plane->getMeshComp()->mVertices;
    std::vector<GLuint>& indices = plane->getMeshComp()->mIndices;
    gsl::Vector3D ballCoords = getTransformComp()->mMatrix.getPosition();
    gsl::Vector3D newPosition;
    gsl::Vector3D normal;
    gsl::Matrix4x4 position = new gsl::Matrix4x4;
    position.translate(getTransformComp()->mMatrix.getPosition());
    gsl::Matrix4x4 sscale = new gsl::Matrix4x4;
    sscale.scale(getTransformComp()->Scal);

    for(int i = 0; i < (indices.size() - 2); i += 3)
    {
        gsl::Vector3D p0 = gsl::Vector3D(vertices[indices[i]].get_xyz());
        gsl::Vector3D p1 = gsl::Vector3D(vertices[indices[i + 1]].get_xyz());
        gsl::Vector3D p2 = gsl::Vector3D(vertices[indices[i + 2]].get_xyz());

        gsl::Vector3D baryCoords = ballCoords.barycentricCoordinates(p0, p1, p2);
        if (baryCoords.x >= 0 && baryCoords.y >= 0 && baryCoords.z >= 0)
        {
            normal = (p1 - p0)^(p2 - p0);
            normal.normalize();
            acceleration = force * normal * normal.z;

            if(i == 0) //reverse the acceleration when it passes center
                acceleration = gsl::Vector3D(-acceleration.x, -acceleration.y, -acceleration.z);
            mVelocity = mVelocity + acceleration * dt;

            float zOffset = 0.25f;
            newPosition = position.getPosition() + mVelocity;
            newPosition.z = p0.z * baryCoords.x + p1.z * baryCoords.y + p2.z * baryCoords.z;
            position.setPosition(newPosition.x, newPosition.y, newPosition.z + zOffset);
            ballCoords = position.getPosition();
            //qDebug() << "x: " << newPosition.x << "y: " << newPosition.y << " z: " << newPosition.z;
        }
    }

    getTransformComp()->mMatrix = position * sscale;
    //qDebug() << "x: " << getTransformComp()->mMatrix.getPosition().x << "y: " << getTransformComp()->mMatrix.getPosition().y << " z: " << getTransformComp()->mMatrix.getPosition().z;
}

void RollingBall::init()
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &getMeshComp()->mVAO );
    glBindVertexArray( getMeshComp()->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &getMeshComp()->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, getMeshComp()->mVBO );

    glBufferData( GL_ARRAY_BUFFER, getMeshComp()->mVertices.size()*sizeof(Vertex), getMeshComp()->mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)( 6 * sizeof( GLfloat ) ));
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &getMeshComp()->mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getMeshComp()->mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getMeshComp()->mIndices.size() * sizeof(GLuint), getMeshComp()->mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void RollingBall::draw()
{
   glBindVertexArray( getMeshComp()->mVAO );
   glUniformMatrix4fv( getMeshComp()->mMatrixUniform, 1, GL_TRUE, getTransformComp()->mMatrix.constData());
   glDrawArrays(GL_TRIANGLES, 0, getMeshComp()->mVertices.size());//mVertices.size());
}

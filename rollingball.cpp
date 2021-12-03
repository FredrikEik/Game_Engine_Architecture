#include "rollingball.h"

RollingBall::RollingBall(int n, GameObject* p) : OctahedronBall (n)
{
    force.y = -9.81f;
    plane = p;
    mTexture = 3;
}

RollingBall::~RollingBall()
{

}

void RollingBall::move(float dt)
{
    dt = dt/100000;

    std::vector<Vertex>& vertices = plane->getMeshComp()->mVertices;
    std::vector<GLuint>& indices = plane->getMeshComp()->mIndices;
    gsl::Matrix4x4 position = new gsl::Matrix4x4;
    position.translate(getTransformComp()->mMatrix.getPosition());
    gsl::Matrix4x4 sscale = new gsl::Matrix4x4;
    sscale.scale(getTransformComp()->Scal);
    gsl::Vector3D ballCoords = position.getPosition();
    //qDebug() << "ball x: " << ballCoords.x << "y: " << ballCoords.y << " z: " << ballCoords.z;

    for(int i = 0; i < (indices.size() - 2); i += 3)
    {
        // gets triangle
        gsl::Vector3D p0 = gsl::Vector3D(vertices[indices[i]].get_xyz());
        gsl::Vector3D p1 = gsl::Vector3D(vertices[indices[i + 1]].get_xyz());
        gsl::Vector3D p2 = gsl::Vector3D(vertices[indices[i + 2]].get_xyz());

        // gets barysentric coordinates to determine if the ball is above current triangle
        gsl::Vector3D baryCoords = ballCoords.barycentricCoordinates(p0, p1, p2);
        //qDebug() << "x: " << baryCoords.x << "y: " << baryCoords.y << " z: " << baryCoords.z;
        if (baryCoords.x >= 0 && baryCoords.y >= 0 && baryCoords.z >= 0) // barycentric coordinates are wrong for some reson
        {
            qDebug() << "Hallo!?";
            gsl::Vector3D normal = (p1 - p0)^(p2 - p0);
            normal.normalize();

            float planeHeight = p0.y * baryCoords.x + p1.y * baryCoords.y + p2.y * baryCoords.z;

            if(ballCoords.y < planeHeight + 0.25f) // ball has collided and will be rolling
            {
                acceleration = force * normal * normal.y;

            }
            else // ball is in free fall
            {
                acceleration = force;
            }

            mVelocity = mVelocity + acceleration * dt; // set velocity and translate
            position.translate(mVelocity);
            ballCoords = position.getPosition();
        }
    }

    getTransformComp()->mMatrix = position * sscale;
    getTransformComp()->mTrueScaleMatrix = position;
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

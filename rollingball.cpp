#include "rollingball.h"

RollingBall::RollingBall(GameObject* Terrain)
{
    force = gsl::Vector3D(0.f, -9.81f, 0.f);
    plane = Terrain;
    //mTexture = 3;
}

RollingBall::~RollingBall()
{

}

void RollingBall::move(float dt)
{
    dt = dt/1000000;
    gsl::Vector3D newPosition { 0.f };
    std::vector<Vertex>& vertices = plane->getMeshComp()->mVertices;
    gsl::Vector3D ballCoords = getTransformComp()->mMatrix.getPosition();
    //qDebug() << "ball x: " << ballCoords.x << "y: " << ballCoords.y << " z: " << ballCoords.z;

    for(int i = 0; i < (vertices.size() - 2); i += 3)
    {
        // gets triangle
        gsl::Vector3D p0 = gsl::Vector3D(vertices[i].get_xyz());
        gsl::Vector3D p1 = gsl::Vector3D(vertices[i + 1].get_xyz());
        gsl::Vector3D p2 = gsl::Vector3D(vertices[i + 2].get_xyz());

        // gets barysentric coordinates to determine if the ball is above current triangle
        gsl::Vector3D baryCoords = ballCoords.barycentricCoordinates(p0, p1, p2);
        //qDebug() << "x: " << baryCoords.x << "y: " << baryCoords.y << " z: " << baryCoords.z;
        if (baryCoords.x >= 0 && baryCoords.y >= 0 && baryCoords.z >= 0) // barycentric coordinates are wrong for some reson
        {
            //qDebug() << "Hallo!?";
            gsl::Vector3D normal = (p1 - p0)^(p2 - p0);
            normal.normalize();

            float planeHeight = p0.y * baryCoords.x + p1.y * baryCoords.y + p2.y * baryCoords.z + 0.50f;
            //qDebug() << " PlaneHeight: " << planeHeight + 0.25 << " BallHeight: " << ballCoords.y;
            if(ballCoords.y <= planeHeight) // ball has collided and will be rolling
            {
                //qDebug() << "x: " << normal.x << "y: " << normal.y << " z: " << normal.z;
                acceleration = gsl::Vector3D(-normal.x * normal.y * force.y, -normal.y * normal.y * force.y, -normal.z * normal.y * force.y) + force; //idk why but putting - before normal makes it roll the correct way
                mVelocity = mVelocity + acceleration * dt; // set velocity and translate
                newPosition = ballCoords + mVelocity;
                newPosition.setY(planeHeight - 0.25f);
                //qDebug() << "new pos x: " << newPosition.x << "y: " << newPosition.y << " z: " << newPosition.z;
            }
            else // ball is in free fall
            {
                acceleration = force;
                mVelocity = mVelocity + acceleration * dt/10; // set velocity and translate
                newPosition = ballCoords + mVelocity;
            }

            getTransformComp()->mTrueScaleMatrix.setPosition(newPosition.x, newPosition.y, newPosition.z);
            getTransformComp()->mMatrix = getTransformComp()->mTrueScaleMatrix;
            getTransformComp()->mMatrix.scale(getTransformComp()->Scal);
        }
    }
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

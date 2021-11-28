#include "rollingball.h"
#include "QDebug"
#include "lasterraingenerator.h"


RollingBall::RollingBall(int n) : OctahedronBall(n)
{
    //mVelocity = gsml::Vector3d{1.0f, 1.0f, -0.05f};
    //mPosition.setPosition(-1.0,0.0,1);
    //mScale.scale(0.25,0.25,0.25);
    gForce = gAcceleration * massKG;
}
RollingBall::~RollingBall()
{

}
void RollingBall::move(float x, float y, float z)
{

}
void RollingBall::move(float dt)
{
    std::vector<Vertex>& vertices = dynamic_cast<class LasTerrainGenerator*>(LasTerrainGenerator)->getMeshComponent()->mVertices;

    gsl::Vector3D BallPosition = getTransformComponent()->mMatrix.getPosition();

    for(unsigned long long i = 0; i < vertices.size() - 2; i+=3)
    {
        gsl::Vector3D p1, p2, p3;
        p1 = gsl::Vector3D(vertices[i].getXYZ());
        p2 = gsl::Vector3D(vertices[i+1].getXYZ());
        p3 = gsl::Vector3D(vertices[i+2].getXYZ());

        gsl::Vector3D BaryCord;
        BaryCord = BallPosition.barycentricCoordinates(p1, p2, p3);

        if(BaryCord.x >= 0 && BaryCord.y >= 0 && BaryCord.z >= 0)
        {
            gsl::Vector3D p12     = p2-p1;
            gsl::Vector3D p13     = p3-p1;
            gsl::Vector3D pNormal = p12^p13;
            pNormal.normalize();

            /*gForce.x = abs(gForce.x);
            gForce.y = abs(gForce.y);
            gForce.z = abs(gForce.z);*/

           // acceleration = gForce ^ pNormal ^ gsml::Vec3(0,0,pNormal.z);
            velocity = velocity + acceleration * dt;


            //gsl::Vector3D newPosition = mPosition.getPosition() + velocity;
            //newPosition.z = (p1.z*BaryCord.x + p2.z*BaryCord.y + p3.z*BaryCord.z)+0.25;
            //mPosition.setPosition(newPosition.x, newPosition.y, newPosition.z);

            BallPosition = getTransformComponent()->mMatrix.getPosition();
            //qDebug() << "BallPosition: " << BallPosition.x << BallPosition.y << BallPosition.z;


        }
    }


    //mMatrix = mPosition * mScale;


}
void RollingBall::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &getMeshComponent()->mVAO );
    glBindVertexArray( getMeshComponent()->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &getMeshComponent()->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, getMeshComponent()->mVBO );

    glBufferData( GL_ARRAY_BUFFER, getMeshComponent()->mVertices.size()*sizeof(Vertex), getMeshComponent()->mVertices.data(), GL_STATIC_DRAW );

    glBindBuffer(GL_ARRAY_BUFFER, getMeshComponent()->mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

}

void RollingBall::barycentricCoordinates()
{
}

void RollingBall::draw()
{
    glBindVertexArray( getMeshComponent()->mVAO );
    glDrawArrays(GL_TRIANGLES, 0, getMeshComponent()->mVertices.size());
    glBindVertexArray(0);
}

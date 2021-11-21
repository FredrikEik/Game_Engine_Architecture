#include "rollingball.h"
#include "lassurface.h"

Rollingball::Rollingball(int n) : OctahedronBall(n)
{
    gForce = gAcceleration * massInKg;
}
Rollingball::~Rollingball()
{

}
void Rollingball::move(float dt)
{
    std::vector<Vertex>& vertices = dynamic_cast<class LASsurface*>(LASsurface)->getMeshComponent()->mVertices;

    gsl::Vector3D barycCoords;
    gsl::Vector3D ballPosition = getTransformComponent()->mMatrix.getPosition();

    for(int i = 0; i < vertices.size() - 2; i+= 3)
    {
        gsl::Vector3D p1, p2, p3;
        p1 = gsl::Vector3D(vertices[i].getXYZ());
        p2 = gsl::Vector3D(vertices[i+1].getXYZ());
        p3 = gsl::Vector3D(vertices[i+2].getXYZ());
        //qDebug() << "p1:" << p1.x<<p1.y<<p1.z << " p2:" << p2.x<<p2.y<<p3.z << " p3:" << p3.x<<p3.y<<p3.z;

        barycCoords = ballPosition.barycentricCoordinates(p1, p2, p3);
        qDebug() << i << barycCoords.x << barycCoords.y << barycCoords.z;

        if(barycCoords.x >= 0 && barycCoords.y >= 0 && barycCoords.z >= 0)
        {

            //qDebug() << "pos before:   " << ballPosition.x << ballPosition.y << ballPosition.z;
            //qDebug() << "barycentric index: " i << barycCoords.x << barycCoords.y << barycCoords.z;

            gsl::Vector3D p12 = p2-p1;
            gsl::Vector3D p13 = p3-p1;
            qDebug() << "p12, p13" << p12.x << p12.y << p12.z << p13.x << p13.y << p13.z;
            gsl::Vector3D pNormal = p12^p13;

            //qDebug() << "pNormal not normalized: " << pNormal.x << pNormal.y << pNormal.z;
            pNormal.normalize();
            qDebug() << "pNormal normalized: " << pNormal.x << pNormal.y << pNormal.z;

            gForce = gsl::Vector3D(abs(gForce.x), abs(gForce.y), abs(gForce.z));
            //gsml::Vector3d pz{pNormal.z, pNormal.z, pNormal.z};

            acceleration = gForce ^ pNormal ^ gsl::Vector3D(0, 0, pNormal.z);
            //acceleration = gAcceleration * gsml::Vector3d(pNormal.x*pNormal.z, pNormal.y*pNormal.z, pNormal.z*pNormal.z-1);
            //acceleration = acceleration + friction;
            velocity = velocity + acceleration * dt;

            float zOffset = 0.25f;
            gsl::Vector3D newPosition = getTransformComponent()->mMatrix.getPosition() + velocity * dt;
            newPosition.z = p1.z*barycCoords.x + p2.z*barycCoords.y + p3.z*barycCoords.z;
            getTransformComponent()->mMatrix.setPosition(newPosition.x, newPosition.y, newPosition.z + zOffset);

            ballPosition = getTransformComponent()->mMatrix.getPosition();

            //mPosition.translate(velocity.x, velocity.y, velocity.z);
            //qDebug() << "pos after:    " << ballPosition.x << ballPosition.y << ballPosition.z;
            //qDebug() << "acceleration: " << acceleration.x << acceleration.y << acceleration.z;
            //qDebug() << "velocity:     " << velocity.x << velocity.y << velocity.z;
        }
        //qDebug() << "ballpos: " << ballPosition.x << ballPosition.y << ballPosition.z;
    }
}


void Rollingball::init()
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

void Rollingball::move(float x, float y, float z)
{
    getTransformComponent()->mMatrix.translate(x,y,z);
    getSphereCollisionComponent()->center += gsl::Vector3D(x,y,z);
    //getBoxCollisionComponent()->min += gsl::Vector3D(0.001f,0.001f, -0.001f);
    //getBoxCollisionComponent()->max += gsl::Vector3D(0.001f,0.001f, -0.001f);
}

void Rollingball::draw()
{
   glBindVertexArray( getMeshComponent()->mVAO );
   glUniformMatrix4fv( getMeshComponent()->mMatrixUniform, 1, GL_TRUE, getTransformComponent()->mMatrix.constData());
   glDrawArrays(GL_TRIANGLES, 0, getMeshComponent()->mVertices.size());//mVertices.size());
}

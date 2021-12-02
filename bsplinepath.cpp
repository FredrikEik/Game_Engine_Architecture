#include "bsplinepath.h"
#include "vector3.h"
#include "vertex.h"
#include "vector3d.h"
#include "sphere.h"
#include "trianglesurface.h"
#include "gameobject.h"

bSplinePath::bSplinePath()
{

}

bSplinePath::bSplinePath(GameObject* ball, GameObject* surface)
{
    setOwnerBall(ball);
    setSurface(surface);
    currentPosition = gsl::Vector3D (ownerBall->getTransformComponent()->mMatrix.getPosition());
    makeControlPoints();

}
bSplinePath::~bSplinePath()
{

}

void bSplinePath::init(/*GLint matrixUniform[4]*/)
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

void bSplinePath::move(float x, float y, float z)
{

}

void bSplinePath::makeControlPoints()
{

    for(int k = 0; k<1000; k++)
    {
        float timeStep = 1;



        //qDebug()<<"Ballposition: " <<ballPos.x << ballPos.y << ballPos.z;

        for(int i = 0; i < surface->getMeshComponent()->mVertices.size(); i+=3)
        {
        gsl::Vector3D p1, p2, p3;

        p1 = gsl::Vector3D(surface->getMeshComponent()->mVertices[i].get_xyz());
        p2 = gsl::Vector3D(surface->getMeshComponent()->mVertices[i+1].get_xyz());
        p3 = gsl::Vector3D(surface->getMeshComponent()->mVertices[i+2].get_xyz());
        gsl::Vector3D baryCoords = currentPosition.barycentricCoordinates(p1,p2,p3);
        //qDebug() << "p1: "<< p1 <<"p2: " << p2 << "p3: "<< p3;

        //qDebug() << i << baryCoords.x << baryCoords.y << baryCoords.z;

        if(baryCoords.x >= 0 && baryCoords.y >= 0 && baryCoords.z >= 0)
            {

            gsl::Vector3D p12 = p2-p1;
            gsl::Vector3D p13 = p3-p1;
            gsl::Vector3D planeNormal = p12^p13;
            planeNormal.normalize();

            float surfaceY = p1.y*baryCoords.x + p2.y*baryCoords.y + p3.y*baryCoords.z;

            if(currentPosition.y < surfaceY+1.05)
            {
                Vertex v;
                v.set_xyz(currentPosition);
                getMeshComponent()->mVertices.push_back(v);
                //acceleration = gForce ^ pNormal ^ gsl::Vector3D(0, pNormal.y, 0);
                acceleration = gsl::Vector3D(planeNormal.x*planeNormal.y*9.80565f, planeNormal.y*planeNormal.y*9.80565f, planeNormal.z*planeNormal.y*9.80565f) + gForce;
                //qDebug()<<"Acceleration: " << acceleration;
                velocity = velocity + acceleration * timeStep;
                gsl::Vector3D nextPosition = currentPosition + velocity;
                qDebug() << "På bakken";
                currentPosition = nextPosition;
                v.set_xyz(nextPosition.x, surfaceY+1, nextPosition.z);
                getMeshComponent()->mVertices.push_back(v);
            }
            else
            {
                Vertex v;
                v.set_xyz(currentPosition);
                getMeshComponent()->mVertices.push_back(v);
                acceleration = gForce;
                velocity = velocity + acceleration * timeStep;
                gsl::Vector3D nextPosition = currentPosition + velocity;
                qDebug() << "I lufta";
                currentPosition = nextPosition;
                v.set_xyz(nextPosition);
                getMeshComponent()->mVertices.push_back(v);
            }

            }
        }
        k = k + timeStep;
    }
}

void bSplinePath::draw()
{
    glBindVertexArray( getMeshComponent()->mVAO );
    glDrawArrays(GL_LINES, 0, getMeshComponent()->mVertices.size());
    glBindVertexArray(0);
}


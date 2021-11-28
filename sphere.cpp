#include "Sphere.h"
#include "vertex.h"
#include "vector3d.h"
#include "trianglesurface.h"
#include "objreader.h"


Sphere::Sphere()
{
    getTransformComponent()->mMatrix.setPosition(50,10,50);
    gForce = gAcceleration * mass;

}
Sphere::~Sphere() {}

void Sphere::init(/*GLint matrixUniform[4]*/)
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

void Sphere::move(float x, float y, float z)
{
    std::vector<Vertex>& vertices = (dynamic_cast<TriangleSurface*>(triangle_surface)->getMeshComponent()->mVertices);
    gsl::Vector3D ballPos = this->getTransformComponent()->mMatrix.getPosition();

    for(int i = 0; i <this->getMeshComponent()->mVertices.size(); i+=3)
    {
    gsl::Vector3D p1, p2, p3;

    p1 = gsl::Vector3D(vertices[i].get_xyz());
    p2 = gsl::Vector3D(vertices[i+1].get_xyz());
    p3 = gsl::Vector3D(vertices[i+2].get_xyz());
    gsl::Vector3D baryCoords = ballPos.barycentricCoordinates(p1,p2,p3);
    //qDebug() <<ballPos.x << ballPos.y << ballPos.z;
    //qDebug() << i << baryCoords.x << baryCoords.y << baryCoords.z;

    if(baryCoords.x >= 0 && baryCoords.y >= 0 && baryCoords.z >= 0)
        {
        qDebug() << ballPos.x << ballPos.y << ballPos.z;
        //qDebug() << velocity.x << velocity.y << velocity.z;
        //qDebug()<< i;
        /*
        p1.x = abs(p1.x); p1.y = abs(p1.y); p1.z = abs(p1.z);
        p2.x = abs(p2.x); p2.y = abs(p2.y); p2.z = abs(p2.z);
        p3.x = abs(p3.x); p3.y = abs(p3.y); p3.z = abs(p3.z);
        */

        gsl::Vector3D p12 = p2-p1;
        gsl::Vector3D p13 = p3-p1;
        //qDebug()<< p12.x << p12.y << p12.z;
        //qDebug()<< p13.x << p13.y << p13.z;
        gsl::Vector3D planeNormal = p12^p13;
        //qDebug()<< planeNormal.x << planeNormal.y << planeNormal.z;
        planeNormal.normalize();
        //qDebug()<< planeNormal.x << planeNormal.y << planeNormal.z;
        //gForce = gsml::Vec3(abs(gForce.x), abs(gForce.y), abs(gForce.z));
        acceleration = gForce ^ planeNormal ^ gsl::Vector3D(0,0,planeNormal.z);
        //qDebug()<< gForce.x << gForce.y << gForce.z;
        //qDebug()<< acceleration.x << acceleration.y << acceleration.z;
        //if(i==0){velocity = velocity - acceleration * 0.00017;}
        //else{velocity = velocity + acceleration * 0.00017;}
        velocity = velocity + acceleration * 0.17;
        //qDebug()<< velocity.x << velocity.y << velocity.z;
        gsl::Vector3D newPos = ballPos + velocity;
        newPos.z = (baryCoords.x * p1.z + baryCoords.y * p2.z + baryCoords.z * p3.z)+0.25;
        qDebug() << newPos;
        getTransformComponent()->mMatrix.setPosition(newPos.x,newPos.y,newPos.z);

        //mPosition.translate(velocity.x,velocity.y,velocity.z);
        }
    }

}


void Sphere::draw()
{
    glBindVertexArray( getMeshComponent()->mVAO );
    //glDrawArrays(GL_TRIANGLES, 0, getMeshComponent()->mVertices.size());
    glDrawElements(GL_TRIANGLES, getMeshComponent()->mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}


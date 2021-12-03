#include "rollingball.h"
#include "lassurface.h"
#include "bspline.h"

Rollingball::Rollingball(int n) : OctahedronBall(n)
{
    gForce = gsl::Vector3D(0.f,-9.80565f*massInKg,0.f);
    velocity = {0.f, -gConstant*0.17f, 0.f};
    prevPosition = getTransformComponent()->mMatrix.getPosition();
}
Rollingball::~Rollingball()
{

}
void Rollingball::move(float dt)
{
    std::vector<Vertex>& vertices = dynamic_cast<class LASsurface*>(LASsurface)->getMeshComponent()->mVertices;

    gsl::Vector3D barycCoords;
    gsl::Vector3D ballPosition = getTransformComponent()->mMatrix.getPosition();
    float yOffset = 0.2f;

    for(int i = 0; i < vertices.size() - 2; i+= 3)
    {
        gsl::Vector3D p1, p2, p3;
        p1 = gsl::Vector3D(vertices[i].getXYZ());
        p2 = gsl::Vector3D(vertices[i+1].getXYZ());
        p3 = gsl::Vector3D(vertices[i+2].getXYZ());
        //qDebug() << "p1:" << p1.x<<p1.y<<p1.z << " p2:" << p2.x<<p2.y<<p3.z << " p3:" << p3.x<<p3.y<<p3.z;

        barycCoords = ballPosition.barycentricCoordinates(p1, p2, p3);
        //qDebug() << i << barycCoords.x << barycCoords.y << barycCoords.z;

        if(barycCoords.x >= 0 && barycCoords.y >= 0 && barycCoords.z >= 0)
        {
            //qDebug() << i << barycCoords.x << barycCoords.y << barycCoords.z;

            //qDebug() << "pos before:   " << ballPosition.x << ballPosition.y << ballPosition.z;
            //qDebug() << "barycentric index: " i << barycCoords.x << barycCoords.y << barycCoords.z;

            gsl::Vector3D p12 = p2-p1;
            gsl::Vector3D p13 = p3-p1;
            //qDebug() << "p12, p13" << p12.x << p12.y << p12.z << p13.x << p13.y << p13.z;
            gsl::Vector3D pNormal = p12^p13;

            //qDebug() << "pNormal not normalized: " << pNormal.x << pNormal.y << pNormal.z;

            //pNormal.y = 0.0f;
            pNormal.normalize();
            //qDebug() << "pNormal normalized: " << pNormal.x << pNormal.y << pNormal.z;


            //gForce = gsl::Vector3D(abs(gForce.x), abs(gForce.y), abs(gForce.z));

            float surfaceY = p1.y*barycCoords.x + p2.y*barycCoords.y + p3.y*barycCoords.z;

            if(ballPosition.y < surfaceY + (yOffset*2))
            {
                //acceleration = gForce ^ pNormal ^ gsl::Vector3D(0, pNormal.y, 0);
                acceleration = gsl::Vector3D(pNormal.x*pNormal.y*gConstant, pNormal.y*pNormal.y*gConstant, pNormal.z*pNormal.y*gConstant) + gsl::Vector3D(0,-gConstant, 0);
                velocity = velocity + acceleration * dt;
                gsl::Vector3D newPosition = getTransformComponent()->mMatrix.getPosition() + velocity * dt;
                getTransformComponent()->mMatrix.setPosition(newPosition.x, surfaceY + yOffset, newPosition.z);
            }
            else
            {
                acceleration = gsl::Vector3D(0,-gConstant, 0);
                velocity = velocity + acceleration * dt;
                gsl::Vector3D newPosition = getTransformComponent()->mMatrix.getPosition() + velocity * dt;
                getTransformComponent()->mMatrix.setPosition(newPosition.x, newPosition.y, newPosition.z);
            }
            //acceleration = gAcceleration * gsml::Vector3d(pNormal.x*pNormal.z, pNormal.y*pNormal.z, pNormal.z*pNormal.z-1);
            //acceleration = acceleration + friction;

            //newPosition.y = p1.y*barycCoords.x + p2.y*barycCoords.y + p3.y*barycCoords.z;
            //ballPosition = getTransformComponent()->mMatrix.getPosition();

            //mPosition.translate(velocity.x, velocity.y, velocity.z);
            //qDebug() << "pos after:    " << ballPosition.x << ballPosition.y << ballPosition.z;
            //qDebug() << "acceleration: " << acceleration.x << acceleration.y << acceleration.z;
            //qDebug() << "velocity:     " << velocity.x << velocity.y << velocity.z;
            return;
        }
        //qDebug() << "ballpos: " << ballPosition.x << ballPosition.y << ballPosition.z;
    }
    float randX = rand()%(20+20+1)-20;
    float randZ = rand()%(20+20+1)-20;
    velocity = gsl::Vector3D{0.f, 0.f, 0.f};
    getTransformComponent()->mMatrix.setPosition(randX, 50, randZ);

}

void Rollingball::predictPath(float dt)
{
    std::vector<Vertex>& vertices = dynamic_cast<class LASsurface*>(LASsurface)->getMeshComponent()->mVertices;

    gsl::Vector3D barycCoords;
    float yOffset = 0.2f;

    for(int i = 0; i < vertices.size() - 2; i+= 3)
    {
        gsl::Vector3D p1, p2, p3;
        p1 = gsl::Vector3D(vertices[i].getXYZ());
        p2 = gsl::Vector3D(vertices[i+1].getXYZ());
        p3 = gsl::Vector3D(vertices[i+2].getXYZ());

        barycCoords = prevPosition.barycentricCoordinates(p1, p2, p3);

        if(barycCoords.x >= 0 && barycCoords.y >= 0 && barycCoords.z >= 0)
        {
            gsl::Vector3D p12 = p2-p1;
            gsl::Vector3D p13 = p3-p1;
            gsl::Vector3D pNormal = p12^p13;

            pNormal.normalize();
            float surfaceY = p1.y*barycCoords.x + p2.y*barycCoords.y + p3.y*barycCoords.z;

            predictedAcceleration = gsl::Vector3D(pNormal.x*pNormal.y*gConstant, pNormal.y*pNormal.y*gConstant, pNormal.z*pNormal.y*gConstant) + gsl::Vector3D(0,-gConstant, 0);
            predictedVelocity = predictedVelocity + predictedAcceleration * dt;
            prevPosition = prevPosition + predictedVelocity * dt;
            mPredictedPath.push_back(Vertex(prevPosition.x, surfaceY + yOffset, prevPosition.z, 255, 0, 0));

//            if(prevPosition.y < surfaceY + (yOffset*2))
//            {
//                predictedAcceleration = gsl::Vector3D(pNormal.x*pNormal.y*gConstant, pNormal.y*pNormal.y*gConstant, pNormal.z*pNormal.y*gConstant) + gsl::Vector3D(0,-gConstant, 0);
//                predictedVelocity = predictedVelocity + predictedAcceleration * dt;
//                prevPosition = prevPosition + predictedVelocity * dt;
//                mPredictedPath.push_back(Vertex(prevPosition.x, surfaceY + yOffset, prevPosition.z, 255, 0, 0));
//            }
//            else
//            {
//                predictedAcceleration = gsl::Vector3D(0,-gConstant, 0);
//                predictedVelocity = predictedVelocity + predictedAcceleration * dt;
//                prevPosition = prevPosition + predictedVelocity * dt;
//                mPredictedPath.push_back(Vertex(prevPosition.x, prevPosition.y, prevPosition.z, 255, 0, 0));
//            }
            return;
        }
    }
    createBSpline();
}

void Rollingball::createBSpline()
{
    qDebug() << "createBSpline()";
    bPredictPath = false;
    mSpline = new BSpline(mPredictedPath);
    mSpline->getMaterialComponent()->mShaderProgram = 1;
    mSpline->getMaterialComponent()->mTextureUnit = 0;
    mSpline->getMeshComponent()->mDrawType = GL_LINES;
    mSpline->init();
    bDrawPath = true;
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
    glDrawArrays(GL_TRIANGLES, 0, getMeshComponent()->mVertices.size());
    glBindVertexArray(0);
}

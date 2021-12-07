#include "bspline.h"
#include "lassurface.h"
#include "bspline.h"
#include "rollingball.h"
#include "components.h"

BSpline::BSpline(GameObject *rollingBall)
{
    ballRef = rollingBall;
    surfaceRef = dynamic_cast<Rollingball*>(ballRef)->LASsurface;
    predictPath(0.017f);
    velocity = {0.f, -gConstant*0.17f, 0.f};

    //Temp
    getMeshComponent()->mVertices = controlPoints;
}

BSpline::~BSpline() {}


void BSpline::init()
{
    initializeOpenGLFunctions();

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

    glBindVertexArray(0);
}

void BSpline::move(float x, float y, float z)
{
    getTransformComponent()->mMatrix.translate(x,y,z);
    getSphereCollisionComponent()->center += gsl::Vector3D(x,y,z);
    //getBoxCollisionComponent()->min += gsl::Vector3D(0.001f,0.001f, -0.001f);
    //getBoxCollisionComponent()->max += gsl::Vector3D(0.001f,0.001f, -0.001f);
}

void BSpline::predictPath(float step)
{
    std::vector<Vertex>& vertices = dynamic_cast<class LASsurface*>(surfaceRef)->getMeshComponent()->mVertices;
    std::vector<GLuint>& indices = dynamic_cast<class LASsurface*>(surfaceRef)->getMeshComponent()->mIndices;
    gsl::Vector3D barycCoords;

    float yOffset = 0.2f;
    float predictPeriod = 100.f;
    float predictStep = step;
    currentCPoint = ballRef->getTransformComponent()->mMatrix.getPosition();

    for(float i = 0; i < predictPeriod; i += predictStep)
    {
        for(int i = 0; i < indices.size() - 2; i+= 3)
        {
            gsl::Vector3D p1, p2, p3;
            p1 = gsl::Vector3D(vertices[indices[i]].getXYZ());
            p2 = gsl::Vector3D(vertices[indices[i+1]].getXYZ());
            p3 = gsl::Vector3D(vertices[indices[i+2]].getXYZ());

            barycCoords = currentCPoint.barycentricCoordinates(p1, p2, p3);

            if(barycCoords.x >= 0 && barycCoords.y >= 0 && barycCoords.z >= 0)
            {
                gsl::Vector3D p12 = p2-p1;
                gsl::Vector3D p13 = p3-p1;
                gsl::Vector3D pNormal = p12^p13;

                pNormal.normalize();
                float surfaceY = p1.y*barycCoords.x + p2.y*barycCoords.y + p3.y*barycCoords.z;

                if(currentCPoint.y < surfaceY + (yOffset*2))
                {
                    acceleration = gsl::Vector3D(pNormal.x*pNormal.y*gConstant, pNormal.y*pNormal.y*gConstant, pNormal.z*pNormal.y*gConstant) + gsl::Vector3D(0,-gConstant, 0);
                    velocity = velocity + acceleration * step;
                    currentCPoint = currentCPoint + velocity * step;
                    controlPoints.push_back(Vertex(currentCPoint.x, surfaceY + yOffset, currentCPoint.z, 0, 0, 255));
                }
                else
                {
                    acceleration = gsl::Vector3D(0,-gConstant, 0);
                    velocity = velocity + acceleration * step;
                    currentCPoint = currentCPoint + velocity * step;
                    controlPoints.push_back(Vertex(currentCPoint.x, currentCPoint.y, currentCPoint.z, 0, 0, 255));
                }
            }
        }
    }
}

void BSpline::draw()
{
    glBindVertexArray( getMeshComponent()->mVAO );
    glDrawArrays(getMeshComponent()->mDrawType, 0, getMeshComponent()->mVertices.size());
    glBindVertexArray(0);
}

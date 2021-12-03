#include "bspline.h"
#include "components.h"

BSpline::BSpline(std::vector<Vertex> cPoints)
{
    controlPoints = cPoints;
    getMeshComponent()->mVertices = cPoints;
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


void BSpline::draw()
{
    glBindVertexArray( getMeshComponent()->mVAO );
    glDrawArrays(getMeshComponent()->mDrawType, 0, getMeshComponent()->mVertices.size());
    glBindVertexArray(0);
}

#include "beziercurve.h"

#include <math.h>

BezierCurve::BezierCurve()
{

}

BezierCurve::BezierCurve(QVector2D PointA, QVector2D PointB, QVector2D PointC, QVector2D PointD)
{
    for(float u=0.0; u<1.0; u+=0.0001){
        float xu= pow(1-u,3)*PointA.x()+3*pow(1-u,2)*PointB.x()+3*(1-u)*pow(u,2)*PointC.x()+pow(u,3)*PointD.x();
        float yu= pow(1-u,3)*PointA.y()+3*pow(1-u,2)*PointB.y()+3*(1-u)*pow(u,2)*PointC.y()+pow(u,3)*PointD.y();
        mVertices.push_back(Vertex(xu, 0.0f, yu, 1.0, 0.0, 0.0));
    }

    mMatrix.setToIdentity();
}

BezierCurve::~BezierCurve()
{

}

QVector3D BezierCurve::Bezipoint(QVector2D PointA, QVector2D PointB, QVector2D PointC, QVector2D PointD, double size)
{
    for(float u=0.0; u<size; u+=0.0001){
        bezi.setX(pow(1-u,3)*PointA.x()+3*pow(1-u,2)*PointB.x()+3*(1-u)*pow(u,2)*PointC.x()+pow(u,3)*PointD.x());
        bezi.setZ(pow(1-u,3)*PointA.y()+3*pow(1-u,2)*PointB.y()+3*(1-u)*pow(u,2)*PointC.y()+pow(u,3)*PointD.y());
    }
    return bezi;
}


void BezierCurve::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void BezierCurve::draw()
{
    glBindVertexArray( mVAO );
    glDrawArrays(GL_LINE, 0, mVertices.size());
}

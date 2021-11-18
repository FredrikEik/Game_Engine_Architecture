#include <QDebug>
#include <QImage>
#include <cstdio>
#include <iostream>
#include "trianglesurface.h"
#include "vector3d.h"
#include "matrix4x4.h"
#include "vertex.h"

TriangleSurface::TriangleSurface() : GameObject()
{
    Vertex v{};
    v.set_xyz(0,0,0); v.set_rgb(1,0,0); getMeshComponent()->mVertices.push_back(v);
    v.set_xyz(0.5,0,0); v.set_rgb(0,1,0); getMeshComponent()->mVertices.push_back(v);
    v.set_xyz(0.5,0.5,0); v.set_rgb(0,0,1); getMeshComponent()->mVertices.push_back(v);
    v.set_xyz(0,0,0); v.set_rgb(0,1,0); getMeshComponent()->mVertices.push_back(v);
    v.set_xyz(0.5,0.5,0); v.set_rgb(1,0,0); getMeshComponent()->mVertices.push_back(v);
    v.set_xyz(0,0.5,0); v.set_rgb(0,0,1); getMeshComponent()->mVertices.push_back(v);
}

TriangleSurface::TriangleSurface(std::string filnavn) : GameObject()
{
    readFile(filnavn);
    //mMatrix.setToIdentity();
    //mMatrix.translate(0,0,5);
}

TriangleSurface::~TriangleSurface()
{
    //qDebug() << "TriangleSurface::~TriangleSurface()";
   //delete [] m_vertices;
    //qDebug() << "TriangleSurface::~TriangleSurface() - vertices deleted";
}

void TriangleSurface::readFile(std::string filnavn)
{
    std::ifstream inn;
    std::ifstream inn2;
    inn.open(filnavn.c_str());

//Leser gjennom fil for å sette maximum og minimum kordinater
    if(inn.is_open())
    {
        qDebug() << "reading 1";
        int n;
        inn >> n;

        float currentX;
        float currentY;
        float currentZ;

        inn >> currentX >> currentY >> currentZ;

        mapMinX = currentX;
        mapMaxX = currentX;
        mapMinY = currentY;
        mapMaxY = currentY;

        for(int i=0; i<n; i++)
        {


            inn >> currentX >> currentY >> currentZ;


            if(currentX < mapMinX)
            {
                mapMinX = currentX;
            }
            else if(currentX > mapMaxX)
            {
                mapMaxX = currentX;
            }
            if(currentY < mapMinY)
            {
                mapMinY = currentY;
            }
            else if(currentY > mapMaxY)
            {
                mapMaxY = currentY;
            }

        }
        //qDebug() << mapMinX << mapMaxX << mapMinY << mapMaxY;
        inn.close();
    }
//Bruker maximum og minimums kordinater til å sette opp mappet i et 5x5 rutenett
        float squareSize = 100;

        //Firkanter i x-Retning
        for(float i=mapMinX; i<=mapMaxX; i = i+squareSize)
        {
            qDebug() << "Max X: " << mapMaxX;
            qDebug() <<"Square CoordX: " << i;

            float currentSquareMinX = i;
            float currentSquareMaxX = i+squareSize;
            float combinedHeight = 0;
            int vertexToPush = 1;

            //Firkanter i y-retning
            for(float j=mapMinY; j<=mapMaxY; j = j+squareSize)
            {
                float currentSquareMinY = j;
                float currentSquareMaxY = j+squareSize;
                inn.open(filnavn.c_str());
                if(inn.is_open())
                {

                    int n;
                    inn >> n;
                    float currentX;
                    float currentY;
                    float currentZ;
                    getMeshComponent()->mVertices.reserve(n);
                    float heightsToCombine [210000];
                    int heightsToCombineAmount = 0;
                    Vertex v{};

                    for(int k=0; k<=n; k++)
                    {

                        //Leser inn en linje med kordinater og sjekker om de er innenfor firkanten
                        inn >> currentX >> currentY >> currentZ;
                        //qDebug() << currentY;


                        if(currentX >= currentSquareMinX)
                        {
                            //qDebug() << "1";
                            if (currentX <= currentSquareMaxX)
                            {
                                //qDebug() << "2";
                                if(currentY >= currentSquareMinY)
                                {
                                    //qDebug() << "3";
                                    if(currentY <= currentSquareMaxY)
                                    {
                                        //qDebug() << "MÅL!";
                                        heightsToCombineAmount++;
                                        heightsToCombine[heightsToCombineAmount] = currentZ;
                                        //qDebug() << heightsToCombineAmount;
                                    }
                                }
                            }
                        }
                    }
                    //Legger sammen alle z verdiene fra kordinater i trekanter
                    if(heightsToCombineAmount!= 0)
                    {
                        for(int l=1; l<= heightsToCombineAmount; l++)
                        {
                            combinedHeight = combinedHeight + heightsToCombine[l];
                        }
                        combinedHeight = combinedHeight/heightsToCombineAmount;
                        qDebug() <<"Height: " << combinedHeight;
                    }
                    else
                    {
                        combinedHeight = 550;
                    }
/*
                    gsl::Vector3D vertexOne   = {currentSquareMinX - mapMinX,combinedHeight - 550,currentSquareMinY - mapMinY};
                    gsl::Vector3D vertexTwo   = {currentSquareMaxX - mapMinX,combinedHeight - 550,currentSquareMinY - mapMinY};
                    gsl::Vector3D vertexThree = {currentSquareMinX - mapMinX,combinedHeight - 550,currentSquareMaxY - mapMinY};
                    gsl::Vector3D vertexFour  = {currentSquareMinX - mapMinX,combinedHeight - 550,currentSquareMaxY - mapMinY};
                    gsl::Vector3D vertexFive  = {currentSquareMaxX - mapMinX,combinedHeight - 550,currentSquareMaxY - mapMinY};
                    gsl::Vector3D vertexSix   = {currentSquareMaxX - mapMinX,combinedHeight - 550,currentSquareMinY - mapMinY};
                    vertexOne.normalize();
                    vertexTwo.normalize();
                    vertexThree.normalize();
                    vertexFour.normalize();
                    vertexFive.normalize();
                    vertexSix.normalize();




                    qDebug() << vertexOne.x << vertexOne.y << vertexOne.z;

                    //qDebug() << vertexOne.x << vertexOne.y << vertexOne.z;




                    if(vertexToPush == 1){v.set_xyz(vertexOne.x, vertexOne.y, vertexOne.z); v.set_rgb(1,0,0); getMeshComponent()->mVertices.push_back(v);}
                    else if(vertexToPush == 2){v.set_xyz(vertexTwo.x, vertexTwo.y, vertexTwo.z); v.set_rgb(1,0,0); getMeshComponent()->mVertices.push_back(v);}
                    else if(vertexToPush == 3){v.set_xyz(vertexThree.x, vertexThree.y, vertexThree.z); v.set_rgb(1,0,0); getMeshComponent()->mVertices.push_back(v);}
                    else if(vertexToPush == 4){v.set_xyz(vertexFour.x, vertexFour.y, vertexFour.z); v.set_rgb(1,0,0); getMeshComponent()->mVertices.push_back(v);}
                    else if(vertexToPush == 5){v.set_xyz(vertexFive.x,vertexFive.y, vertexFive.z); v.set_rgb(1,0,0); getMeshComponent()->mVertices.push_back(v);}
                    else if(vertexToPush == 6){v.set_xyz(vertexSix.x, vertexSix.y, vertexSix.z); v.set_rgb(1,0,0); getMeshComponent()->mVertices.push_back(v);}
*/
                inn.close();
                }
        }

    }
}


void TriangleSurface::writeFile(std::string filnavn)
{
    std::ofstream ut;
    ut.open(filnavn.c_str());

    if (ut.is_open())
    {

        auto n = getMeshComponent()->mVertices.size();
        Vertex vertex;
        ut << n << std::endl;
        for (auto it=getMeshComponent()->mVertices.begin(); it != getMeshComponent()->mVertices.end(); it++)
        {
            //vertex = *it;
            ut << *it << std::endl;
        }
        ut.close();
    }
}
void TriangleSurface::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &getMeshComponent()->mVAO );
    glBindVertexArray( getMeshComponent()->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &getMeshComponent()->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, getMeshComponent()->mVBO );

    glBufferData( GL_ARRAY_BUFFER, getMeshComponent()->mVertices.size()*sizeof(Vertex), getMeshComponent()->mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, getMeshComponent()->mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );

    glBindVertexArray(0);
}

void TriangleSurface::draw()
{
    glBindVertexArray( getMeshComponent()->mVAO );
    glDrawArrays(getMeshComponent()->mDrawType, 0, getMeshComponent()->mVertices.size());
    glBindVertexArray(0);
}

void TriangleSurface::construct()
{
   float xmin=0.0f, xmax=1.0f, ymin=0.0f, ymax=1.0f, h=0.25f;
   for (auto x=xmin; x<xmax; x+=h)
       for (auto y=ymin; y<ymax; y+=h)
       {
           float z = sin(M_PI*x)*sin(M_PI*y);
           getMeshComponent()->mVertices.push_back(Vertex{x,y,z,x,y,z});
           z = sin(M_PI*(x+h))*sin(M_PI*y);
           getMeshComponent()->mVertices.push_back(Vertex{x+h,y,z,x,y,z});
           z = sin(M_PI*x)*sin(M_PI*(y+h));
           getMeshComponent()->mVertices.push_back(Vertex{x,y+h,z,x,y,z});
           getMeshComponent()->mVertices.push_back(Vertex{x,y+h,z,x,y,z});
           z = sin(M_PI*(x+h))*sin(M_PI*y);
           getMeshComponent()->mVertices.push_back(Vertex{x+h,y,z,x,y,z});
           z = sin(M_PI*(x+h))*sin(M_PI*(y+h));
           getMeshComponent()->mVertices.push_back(Vertex{x+h,y+h,z,x,y,z});
       }
}

void TriangleSurface::construct_cylinder()
{
    float h=0.5;
    const int SEKTORER=12;
    float t=2*M_PI/SEKTORER;
    getMeshComponent()->mVertices.clear();
    for (int k=0; k<2; k++)
    {
        for (int i=0; i<SEKTORER; i++)
        {
            float x0=cos(i*t);
            float y0=sin(i*t);
            float z0=h*k;
            float x1=cos((i+1)*t);
            float y1=sin((i+1)*t);
            float z2=h*(k+1);
            getMeshComponent()->mVertices.push_back(Vertex{x0,y0,z0,0,0,1,0,0});
            getMeshComponent()->mVertices.push_back(Vertex{x0,y0,z2,0,0,1,1,0});
            getMeshComponent()->mVertices.push_back(Vertex{x1,y1,z0,0,0,1,0,1});
            getMeshComponent()->mVertices.push_back(Vertex{x0,y0,z2,1,1,0,0,1});
            getMeshComponent()->mVertices.push_back(Vertex{x1,y1,z2,1,1,0,1,0});
            getMeshComponent()->mVertices.push_back(Vertex{x1,y1,z0,1,1,0,1,1});
/*            float x0=cos(i*t);
            float y0=sin(i*t);
            float z0=h*k;
            float x1=cos((i+1)*t);
            float y1=sin((i+1)*t);
            float z2=h*(k+1);
            mVertices.push_back(Vertex{x0,y0,z0,0,0,1});
            mVertices.push_back(Vertex{x1,y1,z0,0,0,1});
            mVertices.push_back(Vertex{x0,y0,z2,0,0,1});
            mVertices.push_back(Vertex{x0,y0,z2,1,1,0});
            mVertices.push_back(Vertex{x1,y1,z0,1,1,0});
            mVertices.push_back(Vertex{x1,y1,z2,1,1,0});
*/        }
    }
}

void TriangleSurface::construct_plane()
{
    float dx=2.0;
    float dy=2.0;
    getMeshComponent()->mVertices.clear();
    for (float y=-2.0; y<2.0; y+=dy)
    {
        for (float x=-3.0; x<3.0; x+=dx)
        {
            float x0=x;
            float y0=y;
            float x1=x0+dx;
            float y1=y0+dy;
            getMeshComponent()->mVertices.push_back(Vertex{x0,y0,0,0,0.5,0});
            getMeshComponent()->mVertices.push_back(Vertex{x1,y0,0,0,0.5,0});
            getMeshComponent()->mVertices.push_back(Vertex{x0,y1,0,0,0.5,0});
            getMeshComponent()->mVertices.push_back(Vertex{x0,y1,0,0,0.5,0});
            getMeshComponent()->mVertices.push_back(Vertex{x1,y0,0,0,0.5,0});
            getMeshComponent()->mVertices.push_back(Vertex{x1,y1,0,0,0.5,0});
        }
    }
}
void TriangleSurface::move(float x, float y, float z)
{

}

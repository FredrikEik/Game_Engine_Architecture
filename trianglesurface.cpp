#include <QDebug>
#include <QImage>
#include <cstdio>
#include <iostream>
#include "trianglesurface.h"
#include "vector3d.h"
#include "contourlines.h"
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
    inn.open(filnavn.c_str());
    std::vector <gsl::Vector3D> mapData;

//Leser gjennom fil for å sette maximum og minimum kordinater
    if(inn.is_open())
    {
        //qDebug() << "reading 1";
        int n;
        inn >> n;

        float currentX;
        float currentY;
        float currentZ;

        inn >> currentX >> currentY >> currentZ;

        mapData.push_back(gsl::Vector3D(currentX,currentY,currentZ));

        mapMinX = currentX;
        mapMaxX = currentX;
        mapMinY = currentY;
        mapMaxY = currentY;
        mapMinZ = currentZ;
        mapMaxZ = currentZ;

        for(int i=0; i<n; i++)
        {


            inn >> currentX >> currentY >> currentZ;

            mapData.push_back(gsl::Vector3D(currentX,currentY,currentZ));


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
            if(currentZ < mapMinZ)
            {
                mapMinZ = currentZ;
            }
            else if(currentZ > mapMaxZ)
            {
                mapMaxZ = currentZ;
            }



        }
        //qDebug() << mapMaxZ;
        //qDebug() << mapMinZ;
        //qDebug() << mapMinX << mapMaxX << mapMinY << mapMaxY;
        inn.close();
    }

//Bruker maximum og minimums kordinater til å sette opp mappet i et 5x5 rutenett
        float squareSize = 5;
        std::vector<float> mapHeights;
        int terrainGridX = 0;
        int terrainGridZ = 0;

        //Firkanter i x-Retning
        for(float i=mapMinX; i<=mapMaxX; i = i+squareSize)
        {
            //qDebug() << "Max X: " << mapMaxX;
            //qDebug() <<"Square CoordX: " << i;
            terrainGridZ = 0;

            float currentSquareMinX = i;
            float currentSquareMaxX = i+squareSize;
            float combinedHeight = 0;


            //Firkanter i y-retning
            for(float j=mapMinY; j<=mapMaxY; j = j+squareSize)
            {

                float currentSquareMinY = j;
                float currentSquareMaxY = j+squareSize;

                    float currentX;
                    float currentY;
                    float currentZ;

                    float heightsToCombine [21000];
                    heightsToCombine [0] = mapMinZ;
                    int heightsToCombineAmount = 0;


                    for(int k=0; k<mapData.size(); k++)
                    {


                        //Leser inn en linje med kordinater og sjekker om de er innenfor firkanten
                        //qDebug() << currentY;
                        currentX = mapData[k].getX();
                        currentY = mapData[k].getY();
                        currentZ = mapData[k].getZ();




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
                                    }
                                }
                            }
                        }
                    }

                    //Legger sammen alle z verdiene fra kordinater i trekanter
                        combinedHeight = 0;
                        for(int l=1; l<= heightsToCombineAmount; l++)
                        {
                            combinedHeight = combinedHeight + heightsToCombine[l];
                           //qDebug() << "Height to add: " << heightsToCombine[l];
                        }
                        //qDebug() << "Height combined: " << combinedHeight;
                        //qDebug() << "Divide amount: " << heightsToCombineAmount;
                        combinedHeight = combinedHeight/heightsToCombineAmount;
                        /*if(combinedHeight < mapMinZ)
                        {
                            qDebug() << "Height combined: " << combinedHeight*heightsToCombineAmount;
                            qDebug() << "Divide amount: " << heightsToCombineAmount;
                            qDebug() << "Result: " << combinedHeight;
                        }
                        */
                        //qDebug() << "Result: " << combinedHeight;
                        //qDebug() <<"Height: " << combinedHeight;
                        mapHeights.push_back(combinedHeight);



                terrainGridZ++;
                //qDebug() << terrainGridZ;
                }
            //qDebug() << terrainGridZ;
            terrainGridX++;
            //qDebug() << terrainGridX;
        }

        //Normalize height

        for (int i = 0; i<mapHeights.size()-5; i++)
        {
            //qDebug() << mapMaxZ;
            //qDebug() << mapMinZ;
            if(isnan(mapHeights[i]))
            {
                mapHeights[i] = mapHeights[i+1];
            }
            //qDebug() << "Vertex " << i << ": " << mapHeights[i];
            float zMaxNormalized = 20;
            float zMinNormalized = 0;
            float tempZ = abs(zMinNormalized + zMinNormalized + ((mapHeights[i]- mapMinZ)*(zMaxNormalized-zMinNormalized))/(mapMaxZ - mapMinZ));
            mapHeights[i] = tempZ;

            //qDebug() << mapHeights[i];
        }

        for(int i = 0; i < terrainGridX; i++)
        {
            for(int j = 0; j < terrainGridZ; j++)
            {
                if(j+((i+1)*terrainGridX) < mapHeights.size() - 100)
                {
                Vertex v{};
                /*Bottom Right*/  v.set_xyz((i*squareSize)  ,mapHeights[j + i*terrainGridZ]      , (j*squareSize)); v.set_rgb(1,0,0); getMeshComponent()->mVertices.push_back(v);
                /*Top Right*/  v.set_xyz((i*squareSize)  ,mapHeights[(j+1) + (i*terrainGridZ)]  , ((j+1)*squareSize)); v.set_rgb(1,0,255); getMeshComponent()->mVertices.push_back(v);
                /*Bottom Left*/  v.set_xyz(((i+1)*squareSize),mapHeights[j + ((i+1)*terrainGridZ)]  , (j*squareSize)); v.set_rgb(1,255,0); getMeshComponent()->mVertices.push_back(v);
                /*Bottom Left*/  v.set_xyz(((i+1)*squareSize),mapHeights[j + ((i+1)*terrainGridZ)]  , (j*squareSize)); v.set_rgb(1,0,255); getMeshComponent()->mVertices.push_back(v);
                /*Top Right*/  v.set_xyz((i*squareSize)  ,mapHeights[(j+1) + (i*terrainGridZ)]  , ((j+1)*squareSize)); v.set_rgb(1,255,0); getMeshComponent()->mVertices.push_back(v);
                /*Top Left*/  v.set_xyz(((i+1)*squareSize),mapHeights[(j+1) + ((i+1)*terrainGridZ)], ((j+1)*squareSize)); v.set_rgb(255,0,0); getMeshComponent()->mVertices.push_back(v);

                }

            }

        }
       /* for(int i = 0; i < getMeshComponent()->mVertices.size(); i++)
        {
            qDebug() << "Vertex nr. " << i << ": " << getMeshComponent()->mVertices[i].get_xyz();
        }
        */
//
        //Countor lines

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
/*          float x0=cos(i*t);
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

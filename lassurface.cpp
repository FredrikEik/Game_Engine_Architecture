#include <QDebug>
#include <QImage>
#include <cstdio>
#include <iostream>
#include <vector>
#include "lassurface.h"


LasSurface::LasSurface(std::string filnavn)
{
    readLasFile(filnavn);
}

LasSurface::~LasSurface()
{

}

void LasSurface::readLasFile(std::string filnavn)
{
    std::ifstream inn;
    inn.open(filnavn.c_str());

    int n = 0;
    int linesRead = 0;
    float x = 0;
    float y = 0;
    float z = 0;

    if (inn.is_open())
    {
        inn >> n;
        inn >> x >> z >> y;
        xMaximum = x;
        xMinimum = x;
        yMaximum = y;
        yMinimum = y;
        zMaximum = z;
        zMinimum = z;
        lasVertices.reserve(n);
        lasVertex.set_xyz(x,y,z);
        lasVertices.push_back(lasVertex);
        for (int i=0; i<n; i++)
        {
            inn >> x >> z >> y;
            linesRead++;
            lasVertex.set_xyz(x,y,z);
            lasVertices.push_back(lasVertex);
            if (x > xMaximum)
                xMaximum = x;
            else if(x < xMinimum)
                xMinimum = x;
            if (y > yMaximum)
                yMaximum = y;
            else if(y < yMinimum)
                yMinimum = y;
            if(z > zMaximum)
                zMaximum = z;
            else if(z < zMinimum)
                zMinimum = z;
        }
        inn.close();
    }

    minMaxScale();


    int VerticesCounter=0;
    int step = 20;
    int squaresDirection = (step-1);
    int Amountsquares = squaresDirection*squaresDirection;
    float xOffset = (xmax-xmin)/step;
    float zOffset = (zmax-zmin)/step;
    double squareMinZ =zmin;
    double squareMaxZ;
    double squareMinX = xmin;
    double squareMaxX;

    for (double j=squareMinZ; j<=zmax-zOffset; j+=zOffset){
        squareMaxZ = j+zOffset;

        for (double i =squareMinX; i<= xmax-xOffset; i+=xOffset){
            numberofPointsInside=0;
            float tempX=0;
            float tempY=0;
            float tempZ=0;
            VerticesCounter++;
            squareMaxX = i +xOffset;
            for ( unsigned long long k = 0 ; k<lasVertices.size(); k++){
                if ( lasVertices[k].getXYZ().getX() < squareMaxX &&
                     lasVertices[k].getXYZ().getX() > i &&
                     lasVertices[k].getXYZ().getZ() < squareMaxZ &&
                     lasVertices[k].getXYZ().getZ() > j){
                    numberofPointsInside++;

                    tempY = tempY+ lasVertices[k].getXYZ().y;
                }
            }
            tempX = squareMaxX - (xOffset/2);
            tempZ =  squareMaxZ - (zOffset/2);
            if(numberofPointsInside==0){
                tempY = (ymin+(ymax-ymin))/2;
            }
            else if(numberofPointsInside > 0)  {

                tempY = tempY/numberofPointsInside;
            }
            vertex.set_xyz(tempX, tempY, tempZ);
            vertex.set_rgb(0,255,100);
            vertex.set_normal(tempX,0,1);

            //qDebug () << numberofPointsInside;
            //qDebug() << vertex.getXYZ().x << vertex.getXYZ().y << vertex.getXYZ().z;
            //qDebug() << lasVertices[squarecounter].getXYZ().x << lasVertices[squarecounter].getXYZ().y << lasVertices[squarecounter].getXYZ().z
            tempVertices.push_back(vertex);

        }
    }

    qDebug() <<"Number of Vertices" << VerticesCounter;
    qDebug() << "Amount of squares" << Amountsquares;
    int k=1;
    int j=0;
        for(int i =0; i< Amountsquares; i++){

            if(i == squaresDirection*k){
                j++;
                k++;
            }

        getMeshComponent()->mVertices.push_back(tempVertices[(i)+(j)]);
        getMeshComponent()->mVertices.push_back(tempVertices[(i+1)+j]);
        getMeshComponent()->mVertices.push_back(tempVertices[(i+step)+j]);

        getMeshComponent()->mVertices.push_back(tempVertices[(i+(step+1)+j)]);
        getMeshComponent()->mVertices.push_back(tempVertices[(i+step)+j]);
        getMeshComponent()->mVertices.push_back(tempVertices[(i+1)+j]);
    }

    //              sortering algoritme, ikke i bruk
    //        for (l = 0; l < n-1; l++)
    //        {
    //            min_index = l;
    //            for (j=l+1 ; j<n; j++)
    //            {
    //                if (a[j] < a[min_index]){
    //                 min_index = j;
    //                }
    //            }

    //qDebug() << xMinimum << xMaximum << yMinimum << yMaximum << zMinimum << zMaximum;
    for(int i =0;i<tempVertices.size();i++){
        qDebug() << tempVertices[i].getXYZ().x << tempVertices[i].getXYZ().y << tempVertices[i].getXYZ().z;
    }


}


void LasSurface::writeFile(std::string filnavn)
{
    std::ofstream ut;
    ut.open(filnavn.c_str());

    if (ut.is_open())
    {

        auto n = mVertices.size();
        Vertex vertex;
        ut << n << std::endl;
        for (auto it=mVertices.begin(); it != mVertices.end(); it++)
        {
            //vertex = *it;
            ut << *it << std::endl;
        }
        ut.close();
    }
}

void LasSurface::minMaxScale()
{
    for(int index = 0; index < lasVertices.size(); index++){

        float scaledX = xmin+(((lasVertices[index].getXYZ().getX()-xMinimum)*(xmax-xmin))/(xMaximum-xMinimum));
        float scaledY = ymin+(((lasVertices[index].getXYZ().getY()-yMinimum)*(ymax-ymin))/(yMaximum-yMinimum));
        float scaledZ = zmin+(((lasVertices[index].getXYZ().getZ()-zMinimum)*(zmax-zmin))/(zMaximum-zMinimum));


        lasVertices[index].set_xyz(scaledX, scaledY, scaledZ);
    }
}

void LasSurface::init()
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();


    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &getMeshComponent()->mVAO );
    glBindVertexArray( getMeshComponent()->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &getMeshComponent()->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, getMeshComponent()->mVBO );

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER,
                  getMeshComponent()->mVertices.size()*sizeof( Vertex ),
                  getMeshComponent()->mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0  );          // array buffer offset
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void LasSurface::draw()
{
    glBindVertexArray(getMeshComponent()->mVAO);
    glDrawArrays(GL_TRIANGLES, 0, getMeshComponent()->mVertices.size());
    glBindVertexArray(0);
}

void LasSurface::move(float x, float y, float z)
{
    getTransformComponent()->mMatrix.translate(x,y,z);
    getSphereCollisionComponent()->center += gsl::Vector3D(x,y,z);
}



#include "planeimport.h"
#include "vertex.h"
#include "MathStuff/MathStuff.h"

#include "components.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <math.h>

PlaneImport::PlaneImport(std::string filename) : GameObject()
{
    readFile(filename);
    minMaxNormalize();

    int quadX = 0;
    int quadZ = 0;

    const int amountOfQuadsZ = abs(zMax-zMin)/step;

    std::array<std::vector<float>, 1800> heights;

    float averageHeights[1800];

    for(int i = 0; i < mPointsArray.size(); i++)
    {
        for(int j = xMin; j < xMax; j += step)
        {
            if(mPointsArray[i].get_xyz().getX() > j && mPointsArray[i].get_xyz().getX() < j + step)
            {
                quadX = (j-xMin)/step;
            }

        }

        for(int j = zMin; j < zMax; j += step)
        {
            if(mPointsArray[i].get_xyz().getZ() > j && mPointsArray[i].get_xyz().getZ() < j + step)
            {
                quadZ = (j-zMin)/step;
            }

        }

        //Konverterer fra rekke og kolonne til vector array indexen
        int vectorIndex = quadZ*amountOfQuadsZ + quadX;

        //pusher back høydekoordinatene til rett vector i arrayet
        heights[vectorIndex].push_back(mPointsArray[i].get_xyz().getY());

    }

    for(int i = 0; i < heights.size(); i++)
    {
        float sum = 0;

        for(int j = 0; j < heights[i].size(); j++)
        {
            sum += heights[i][j];
        }
        sum = sum / heights[i].size();
        averageHeights[i] = sum;
    }

    float R, G, B = 1;

    for(float x = xMin; x < xMax - step; x += step)
    {
        for(float z = zMin; z < zMax; z += step)
        {
            int quadX = (x - xMin) / step;
            int quadZ = (z - zMin) / step;

            getMeshComp()
                    ->mVertices.push_back(
                        Vertex(x, averageHeights[quadZ*amountOfQuadsZ + quadX], z,
                        R/255, averageHeights[quadZ*amountOfQuadsZ + quadX]*G/255, B/255,
                        0,0));
                    //x, z+1
            getMeshComp()
                    ->mVertices.push_back(
                        Vertex(x, averageHeights[(quadZ+1)*amountOfQuadsZ + quadX], z+step,
                        R/255, averageHeights[(quadZ+1)*amountOfQuadsZ + quadX]*G/255, B/255,
                        0, 1));
                    //x+1,z
            getMeshComp()
                    ->mVertices.push_back(
                        Vertex(x+step, averageHeights[quadZ*amountOfQuadsZ + quadX+1], z,
                        R/255, averageHeights[quadZ*amountOfQuadsZ + quadX+1]*G/255, B/255,
                        1,0));

                    //x+1, z+1
                    getMeshComp()
                    ->mVertices.push_back(
                        Vertex(x+step, averageHeights[(quadZ+1)*amountOfQuadsZ + quadX+1], z+step,
                        R/255, averageHeights[(quadZ+1)*amountOfQuadsZ + quadX+1]*G/255, B/255,
                        1, 1));

                    //x+1, z
                    getMeshComp()
                    ->mVertices.push_back(
                        Vertex(x+step, averageHeights[quadZ*amountOfQuadsZ + quadX+1], z,
                        R/255, averageHeights[quadZ*amountOfQuadsZ + quadX+1]*G/255, B/255,
                        1,0));

                    //x, z+1
                    getMeshComp()
                    ->mVertices.push_back(
                        Vertex(x, averageHeights[(quadZ+1)*amountOfQuadsZ + quadX], z+step,
                        R/255, averageHeights[(quadZ+1)*amountOfQuadsZ + quadX]*G/255, B/255,
                        0, 1));



        }
    }
}

PlaneImport::~PlaneImport()
{

}
void PlaneImport::init()
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();


    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &getMeshComp()->mVAO );
    glBindVertexArray( getMeshComp()->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &getMeshComp()->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, getMeshComp()->mVBO );

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER,
                  getMeshComp()->mVertices.size()*sizeof( Vertex ),
                  getMeshComp()->mVertices.data(), GL_STATIC_DRAW );

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

void PlaneImport::draw()
{
    glBindVertexArray( getMeshComp()->mVAO );
    glDrawArrays(GL_TRIANGLES, 0, getMeshComp()->mVertices.size());
    glBindVertexArray(0);
}

void PlaneImport::readFile(std::string filename)
{
    //Leser Fila
    std::ifstream inn;
    inn.open(filename.c_str());
    bool first = true;
    if (!inn.is_open())
    {
        qDebug() << "Error, file did not open";
    }
    if (inn.is_open()) {
        int numberOfVertices;
        inn >> numberOfVertices;
        mPointsArray.reserve(numberOfVertices);
        Vertex vertex{0,0,0};
        double tempX, tempY, tempZ;

        for (int i=0; i < numberOfVertices; i++)
        {

             inn >> tempX;
             inn >> tempZ;
             inn >> tempY;
             //convert to float
             x = tempX;
             y = tempY;
             z = tempZ;

             if(first)
             {
                 maxPointX = x;
                 maxPointY = y;
                 maxPointZ = z;
                 minPointX = x;
                 minPointY = y;
                 minPointZ = z;
                 first = false;
             }

             if(x > maxPointX)
             {
                 maxPointX = x;
             }
             if(y > maxPointY)
             {
                 maxPointY = y;
             }
             if(z > maxPointZ)
             {
                 maxPointZ = z;
             }
             if(x < minPointX)
             {
                 minPointX = x;
             }
             if(y < minPointY)
             {
                 minPointY = y;
             }
             if(z < minPointZ)
             {
                 minPointZ = z;
             }

             vertex.set_xyz(x, y, z);


             mPointsArray.push_back(vertex);
        }
        inn.close();
    }
}

void PlaneImport::minMaxNormalize()
{
    for(int i = 0; i < mPointsArray.size(); i++)
    {
        float nx = xMin+(((mPointsArray[i].get_xyz().getX() - minPointX)*(xMax-xMin)) / (maxPointX - minPointX));
        float ny = yMin+(((mPointsArray[i].get_xyz().getY() - minPointY)*(yMax-yMin)) / (maxPointY - minPointY));
        float nz = zMin+(((mPointsArray[i].get_xyz().getZ() - minPointZ)*(zMax-zMin)) / (maxPointZ - minPointZ));
        //qDebug() << "NY: " << ny;
        mPointsArray[i].set_xyz(nx,ny,nz);

    }

}

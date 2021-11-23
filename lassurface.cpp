#include "lassurface.h"
#include <QDebug>
#include "numeric"
#include "vector"

LASsurface::LASsurface(std::string fileName)
{
    readLAS(fileName);
    minMaxNormalize();

    // Coordinates for which quad the vertex is on
    int quadCoordX = 0;
    int quadCoordZ = 0;

    // Using min, max and step to find how many quads the TriangleSurface has in each direction
    const int quadAmountX = abs(xMax-xMin)/step; //qDebug() << quadAmountX;
    const int quadAmountZ = abs(zMax-zMin)/step; //qDebug() << quadAmountZ;
    //const int numOfQuads = quadAmountX*quadAmountZ; //qDebug() << numOfQuads;

    // An array of vectors that hold all the heights for each quad
    // Size of array is hardcoded, unsure how to do it from numOfQuads
    std::array<std::vector<float>,800> heights;
    //std::array<std::vector<float>,numOfQuads> heights;

    //An array that holds the average height of each quad
    float averageHeights[800];
    //float averageHeights[numOfQuads];



    //For every vertices
    //qDebug() << "Vertices size" << lasData.size();
    for(int i = 0; i < lasData.size(); i++)
    {
        //.. find what column the vertex is on
        for(int j = xMin; j < xMax; j+=step) // j = -5.0f, -3.0f, -1.0f, 1.0f, 3.0f, 5.0f
        {
            if(lasData[i].getXYZ().getX() > j && lasData[i].getXYZ().getX() < j + step)
            {
                quadCoordX = (j-xMin)/step;
            }
        }
        //.. find what row the vertex is on
        for(int j = zMin; j < zMax; j+=step)
        {
            if(lasData[i].getXYZ().getZ() > j && lasData[i].getXYZ().getZ() < j + step)
            {
                quadCoordZ = (j-zMin)/step;
            }
        }

        //Converts from row&column to vector-array index
        int vectorIndex = quadCoordZ*quadAmountZ + quadCoordX; //0-1199

        //Pushes the height(y) to the correct vector in the array
        heights[vectorIndex].push_back(lasData[i].getXYZ().getY());
    }

    //qDebug() << "size of height array " << heights.size();
    for(int i = 0; i < heights.size(); i++)
    {
        //qDebug() << "size of height vector " << i << heights[i].size();

        //Calculate the average of all heights in quad
        //.. and put it in the array of averageHeights

        //float sum = std::accumulate(heights[i].begin(), heights[i].end(), 0);

        float sum = 0;
        for(int j = 0; j < heights[i].size(); j++)
        {
            sum += heights[i][j];
        }
        sum = sum/heights[i].size();
        averageHeights[i] = sum;
    }

//    for(int i = 0; i < 25 ; i++)
//    {
//        qDebug() << "averageheight:" << i << " " << averageHeights[i];
//    }

    //Create triangulated surface
    float R = 10;
    float G = 50;
    float B = 75;
    for (float x = xMin; x < xMax-step; x+= step)
    {
        for(float z = zMin; z < zMax; z+= step)
        {
            int quadCoordX = (x-xMin)/step;
            int quadCoordZ = (z-zMin)/step;
            float u{(x + abs(xMin)) / (xMax + abs(xMin) + step)};
            float v{(z + abs(zMin)) / (zMax + abs(zMin) + step)};

//            getMeshComponent()->mVertices.push_back(Vertex(x, averageHeights[quadCoordZ*quadAmountZ + quadCoordX], z,
//                                                           R/255, averageHeights[quadCoordZ*quadAmountZ + quadCoordX]*G/255, B/255,u,v));

//            getMeshComponent()->mVertices.push_back(Vertex(x+step, averageHeights[(quadCoordZ+1)*quadAmountZ + quadCoordX+1], z+step,
//                                                           R, averageHeights[(quadCoordZ+1)*quadAmountZ + quadCoordX+1]*G/255, z+step, u+step, v+step));

//            getMeshComponent()->mVertices.push_back(Vertex(x+step, averageHeights[quadCoordZ*quadAmountZ + quadCoordX+1], z,
//                                                           R/255, averageHeights[quadCoordZ*quadAmountZ + quadCoordX+1]*G/255, B/255, u+step,v));

//            getMeshComponent()->mVertices.push_back(Vertex(x, averageHeights[quadCoordZ*quadAmountZ + quadCoordX], z,
//                                                           R/255, averageHeights[quadCoordZ*quadAmountZ + quadCoordX]*G/255, B/255, u, v));

//            getMeshComponent()->mVertices.push_back(Vertex(x, averageHeights[(quadCoordZ+1)*quadAmountZ + quadCoordX], z+step,
//                                                           R/255, averageHeights[(quadCoordZ+1)*quadAmountZ + quadCoordX]*G/255, B/255, u, v+step));

//            getMeshComponent()->mVertices.push_back(Vertex(x+step, averageHeights[(quadCoordZ+1)*quadAmountZ + quadCoordX+1], z+step,
//                                                           R/255, averageHeights[(quadCoordZ+1)*quadAmountZ + quadCoordX+1]*G/255, B/255, u+step, v+step));


            getMeshComponent()->mVertices.push_back(Vertex(x, averageHeights[quadCoordZ*quadAmountZ + quadCoordX], z,
                                                           R/255, averageHeights[quadCoordZ*quadAmountZ + quadCoordX]*G/255, B/255,u,v));

            getMeshComponent()->mVertices.push_back(Vertex(x, averageHeights[(quadCoordZ+1)*quadAmountZ + quadCoordX], z+step,
                                                           R/255, averageHeights[(quadCoordZ+1)*quadAmountZ + quadCoordX]*G/255, B/255, u, v+step));

            getMeshComponent()->mVertices.push_back(Vertex(x+step, averageHeights[quadCoordZ*quadAmountZ + quadCoordX+1], z,
                                                           R/255, averageHeights[quadCoordZ*quadAmountZ + quadCoordX+1]*G/255, B/255, u+step,v));



            getMeshComponent()->mVertices.push_back(Vertex(x+step, averageHeights[(quadCoordZ+1)*quadAmountZ + quadCoordX+1], z+step,
                                                           R/255, averageHeights[(quadCoordZ+1)*quadAmountZ + quadCoordX+1]*G/255, B/255, u+step, v+step));

            getMeshComponent()->mVertices.push_back(Vertex(x+step, averageHeights[quadCoordZ*quadAmountZ + quadCoordX+1], z,
                                                           R/255, averageHeights[quadCoordZ*quadAmountZ + quadCoordX+1]*G/255, B/255, u+step,v));

            getMeshComponent()->mVertices.push_back(Vertex(x, averageHeights[(quadCoordZ+1)*quadAmountZ + quadCoordX], z+step,
                                                           R/255, averageHeights[(quadCoordZ+1)*quadAmountZ + quadCoordX]*G/255, B/255, u, v+step));



        }
    }


    /* Print all vertices in trianglesurface*/
//    for(int i = 0; i < getMeshComponent()->mVertices.size(); i++)
//    {
//        qDebug() << getMeshComponent()->mVertices[i].getXYZ().getX()
//                 << getMeshComponent()->mVertices[i].getXYZ().getY()
//                 << getMeshComponent()->mVertices[i].getXYZ().getZ();
//    }
}

LASsurface::~LASsurface()
{

}


void LASsurface::readLAS(std::string fileName)
{
    std::ifstream inn;
    inn.open(fileName.c_str());

    if (!inn.is_open())
    {
        qDebug() << "Error, file couldn't be opened";
    }
    if (inn.is_open()) {
        int numberOfVertices;
        inn >> numberOfVertices;
        lasData.reserve(numberOfVertices);
        Vertex vertex{0,0,0};
        double tempX, tempY, tempZ;
        float x,y,z;
        for (int i=0; i < numberOfVertices; i++)
        {

             inn >> tempX >> tempZ >> tempY;
             //convert double to float since las data has a lot of decimals
             x = float(tempX);
             y = float(tempY);
             z = float(tempZ);

             if(i == 0)
             {
                 highestX = x;
                 highestY = y;
                 highestZ = z;
                 lowestX = x;
                 lowestY = y;
                 lowestZ = z;
             }

             if(tempX > highestX){ highestX = x;}
             if(tempY > highestY){ highestY = y;}
             if(tempZ > highestZ){ highestZ = z;}
             if(tempX < lowestX){ lowestX = x;}
             if(tempY < lowestY){ lowestY = y;}
             if(tempZ < lowestZ){ lowestZ = z;}

             vertex.set_xyz(x, y, z);
             lasData.push_back(vertex);
        }
        inn.close();
    }

    /*Print las data*/
    qDebug() << highestX << highestY << highestZ << lowestX << lowestY << lowestZ;
//    for(int i = 0; i < 1000; i++)
//    {
//        qDebug() << lasData[i].getXYZ().getX() << lasData[i].getXYZ().getY() << lasData[i].getXYZ().getZ() ;
//    }
}

void LASsurface::minMaxNormalize()
{
    for(int i = 0; i < lasData.size(); i++)
    {
        float nX = xMin+(((lasData[i].getXYZ().getX() - lowestX)*(xMax-xMin)) / (highestX - lowestX));
        float nY = yMin+(((lasData[i].getXYZ().getY() - lowestY)*(yMax-yMin)) / (highestY - lowestY));
        float nZ = zMin+(((lasData[i].getXYZ().getZ() - lowestZ)*(zMax-zMin)) / (highestZ - lowestZ));

        lasData[i].set_xyz(nX,nY,nZ);
    }

    /*Print normalized las data*/
//    for(int i = 0; i < 10000; i++)
//    {
//        qDebug() << lasData[i].getXYZ().getX() << lasData[i].getXYZ().getY() << lasData[i].getXYZ().getZ() ;
//    }
}

void LASsurface::init()
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

void LASsurface::draw()
{
    glBindVertexArray(getMeshComponent()->mVAO );
    glDrawArrays(GL_TRIANGLES, 0, getMeshComponent()->mVertices.size());
    glBindVertexArray(0);
}

void LASsurface::move(float x, float y, float z)
{
    getTransformComponent()->mMatrix.translate(x,y,z);
    getSphereCollisionComponent()->center += gsl::Vector3D(x,y,z);
}





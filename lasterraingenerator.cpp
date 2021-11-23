#include "lasterraingenerator.h"
#include "numeric"
#include "vector"
#include "QDebug"

LasTerrainGenerator::LasTerrainGenerator(std::string fileName)
{
    readFile(fileName);
    minMaxNormalize();

    // Coordinates for which quad the vertex is on
    int quadX = 0;
    int quadZ = 0;

    // Using min, max and step to find how many quads the TriangleSurface has in each direction

    const int quadAmountZ = abs(zMax-zMin)/step; //qDebug() << quadAmountZ;


    // An array of vectors that hold all the heights for each quad
    // Size of array is hardcoded, unsure how to do it from numOfQuads
    std::array<std::vector<float>,800> heights;

    //An array that holds the average height of each quad
    float averageHeights[800];



    //For every vertices
    //qDebug() << "Vertices size" << mPointsArray.size();
    for(int i = 0; i < mPointsArray.size(); i++)
    {

        //.. find what column the vertex is on
        for(int j = xMin; j < xMax; j+=step) // j = -5.0f, -3.0f, -1.0f, 1.0f, 3.0f, 5.0f
        {
            if(mPointsArray[i].getXYZ().getX() > j && mPointsArray[i].getXYZ().getX() < j + step)
            {
                quadX = (j-xMin)/step;
            }
        }
        //.. find what row the vertex is on
        for(int j = zMin; j < zMax; j+=step)
        {
            if(mPointsArray[i].getXYZ().getZ() > j && mPointsArray[i].getXYZ().getZ() < j + step)
            {
                quadZ = (j-zMin)/step;
            }
        }

        //Converts from row&column to vector-array index
        int vectorIndex = quadZ*quadAmountZ + quadX; //0-1199

        //Pushes the height(y) to the correct vector in the array
        heights[vectorIndex].push_back(mPointsArray[i].getXYZ().getY());


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

    //Create triangulated surface
    float R = 10;
    float G = 50;
    float B = 75;
    for (float x = xMin; x < xMax-step; x+= step)
    {
        for(float z = zMin; z < zMax; z+= step)
        {
            int quadX = (x-xMin)/step;
            int quadZ = (z-zMin)/step;
            float u{(x + abs(xMin)) / (xMax + abs(xMin) + step)};
            float v{(z + abs(zMin)) / (zMax + abs(zMin) + step)};

            getMeshComponent()->mVertices.push_back(Vertex(x, averageHeights[quadZ*quadAmountZ + quadX], z,R/255, averageHeights[quadZ*quadAmountZ + quadX]*G/255, B/255,u,v));
            getMeshComponent()->mVertices.push_back(Vertex(x, averageHeights[(quadZ+1)*quadAmountZ + quadX], z+step,R/255, averageHeights[(quadZ+1)*quadAmountZ + quadX]*G/255, B/255, u, v+step));
            getMeshComponent()->mVertices.push_back(Vertex(x+step, averageHeights[quadZ*quadAmountZ + quadX+1], z,R/255, averageHeights[quadZ*quadAmountZ + quadX+1]*G/255, B/255, u+step,v));
            getMeshComponent()->mVertices.push_back(Vertex(x+step, averageHeights[(quadZ+1)*quadAmountZ + quadX+1], z+step, R/255, averageHeights[(quadZ+1)*quadAmountZ + quadX+1]*G/255, B/255, u+step, v+step));
            getMeshComponent()->mVertices.push_back(Vertex(x+step, averageHeights[quadZ*quadAmountZ + quadX+1], z,R/255, averageHeights[quadZ*quadAmountZ + quadX+1]*G/255, B/255, u+step,v));
            getMeshComponent()->mVertices.push_back(Vertex(x, averageHeights[(quadZ+1)*quadAmountZ + quadX], z+step,R/255, averageHeights[(quadZ+1)*quadAmountZ + quadX]*G/255, B/255, u, v+step));
        }
    }
    /* Print all vertices in trianglesurface*/
    for(int i = 0; i < getMeshComponent()->mVertices.size(); i++)
    {
        qDebug() << getMeshComponent()->mVertices[i].getXYZ().getX()
                 << getMeshComponent()->mVertices[i].getXYZ().getY()
                 << getMeshComponent()->mVertices[i].getXYZ().getZ();
    }
}
LasTerrainGenerator::~LasTerrainGenerator()
{}
void LasTerrainGenerator::readFile(std::string filename)
{
    std::ifstream inn;
    inn.open(filename.c_str());
    bool first = true;
    if (!inn.is_open())
    {
        qDebug() << "Error, file couldn't be opened";
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
                 highestX = x;
                 highestY = y;
                 highestZ = z;
                 lowestX = x;
                 lowestY = y;
                 lowestZ = z;
                 first = false;
             }

             if(x > highestX)
             {
                 highestX = x;
             }
             if(y > highestY)
             {
                 highestY = y;
             }
             if(z > highestZ)
             {
                 highestZ = z;
             }
             if(x < lowestX)
             {
                 lowestX = x;
             }
             if(y < lowestY)
             {
                 lowestY = y;
             }
             if(z < lowestZ)
             {
                 lowestZ = z;
             }

             vertex.set_xyz(x, y, z);


             mPointsArray.push_back(vertex);
        }
        inn.close();
        qDebug() << "Done loading las file";
    }
}


void LasTerrainGenerator::init()
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
void LasTerrainGenerator::draw()
{
   // glBindVertexArray( getMeshComponent()->mVAO );
   // glUniformMatrix4fv( getMeshComponent()->mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
   // glDrawArrays(GL_TRIANGLES, 0, getMeshComponent()->mVertices.size());

    glBindVertexArray( getMeshComponent()->mVAO );
    glDrawArrays(GL_TRIANGLES, 0, getMeshComponent()->mVertices.size());
    glBindVertexArray(0);
}
void LasTerrainGenerator::move(float x, float y, float z)
{
    getTransformComponent()->mMatrix.translate(x,y,z);
    getSphereCollisionComponent()->center += gsl::Vector3D(x,y,z);
}




void LasTerrainGenerator::minMaxNormalize()
{
    for(int i = 0; i < mPointsArray.size(); i++)
    {
        float nx = xMin+(((mPointsArray[i].getXYZ().getX() - lowestX)*(xMax-xMin)) / (highestX - lowestX));
        float ny = yMin+(((mPointsArray[i].getXYZ().getY() - lowestY)*(yMax-yMin)) / (highestY - lowestY));
        float nz = zMin+(((mPointsArray[i].getXYZ().getZ() - lowestZ)*(zMax-zMin)) / (highestZ - lowestZ));
        //qDebug() << "NY: " << ny;
        mPointsArray[i].set_xyz(nx,ny,nz);

    }
}

#include "lasterraingenerator.h"
#include "numeric"
#include "vector"
#include "QDebug"

MeshComponent* contourLineMesh;

LasTerrainGenerator::LasTerrainGenerator(std::string fileName)
{
    readFile(fileName);
    minMaxNormalize(); //Thobias Ovastrøm kode

    //kordinater som brukes til å sjekke hvilket quad vertexene er på
    int quadX = 0;
    int quadZ = 0;

    //Quads i hver rettning
    const int amountOfQuads = abs(zMax-zMin)/step;

    //Et Array med alle høyde verdiene til alle quads
    std::array<std::vector<float>,800> heights;

    //Et array med gjennomsnittet av høydene
    float averageHeights[800];

    //For alle vertices..
    for(int i = 0; i < mPointsArray.size(); i++)
    {
        //..finner kolonnen
        for(int j = xMin; j < xMax; j+=step)
        {
            if(mPointsArray[i].getXYZ().getX() > j && mPointsArray[i].getXYZ().getX() < j + step)
            {
                quadX = (j-xMin)/step;
            }
        }
        //..finner rekka
        for(int j = zMin; j < zMax; j+=step)
        {
            if(mPointsArray[i].getXYZ().getZ() > j && mPointsArray[i].getXYZ().getZ() < j + step)
            {
                quadZ = (j-zMin)/step;
            }
        }
        //Konverterer fra rekke og kolonne til vector array indexen
        int vectorIndex = quadZ*amountOfQuads + quadX;

        //push backer høyde kordinatene till rett vector i arrayet
        heights[vectorIndex].push_back(mPointsArray[i].getXYZ().getY());
    }

    for(int i = 0; i < heights.size(); i++)
    {
        //Calculate the average of all heights in quad
        //.. and put it in the array of averageHeights
        float sum = 0;
        for(int j = 0; j < heights[i].size(); j++)
        {
            sum += heights[i][j];
        }
        sum = sum/heights[i].size();
        averageHeights[i] = sum;
    }

    //Lager triangulert overflate her
    float R, B, G = 1; //Må ha rgb

    for (float x = xMin; x < xMax-step; x+= step)
    {
        for(float z = zMin; z < zMax; z+= step)
        {
            int quadX = (x-xMin)/step;
            int quadZ = (z-zMin)/step;
            float u{(x + abs(xMin)) / (xMax + abs(xMin) + step)};
            float v{(z + abs(zMin)) / (zMax + abs(zMin) + step)};

            getMeshComponent()->mVertices.push_back(Vertex(x, averageHeights[quadZ*amountOfQuads + quadX], z,R/255, averageHeights[quadZ*amountOfQuads + quadX]*G/255, B/255,u,v));
            getMeshComponent()->mVertices.push_back(Vertex(x, averageHeights[(quadZ+1)*amountOfQuads + quadX], z+step,R/255, averageHeights[(quadZ+1)*amountOfQuads + quadX]*G/255, B/255, u, v+step));
            getMeshComponent()->mVertices.push_back(Vertex(x+step, averageHeights[quadZ*amountOfQuads + quadX+1], z,R/255, averageHeights[quadZ*amountOfQuads + quadX+1]*G/255, B/255, u+step,v));
            getMeshComponent()->mVertices.push_back(Vertex(x+step, averageHeights[(quadZ+1)*amountOfQuads + quadX+1], z+step, R/255, averageHeights[(quadZ+1)*amountOfQuads + quadX+1]*G/255, B/255, u+step, v+step));
            getMeshComponent()->mVertices.push_back(Vertex(x+step, averageHeights[quadZ*amountOfQuads + quadX+1], z,R/255, averageHeights[quadZ*amountOfQuads + quadX+1]*G/255, B/255, u+step,v));
            getMeshComponent()->mVertices.push_back(Vertex(x, averageHeights[(quadZ+1)*amountOfQuads + quadX], z+step,R/255, averageHeights[(quadZ+1)*amountOfQuads + quadX]*G/255, B/255, u, v+step));
        }
    }
    contourLineMesh = getMeshComponent();
}
LasTerrainGenerator::~LasTerrainGenerator()
{}
void LasTerrainGenerator::readFile(std::string filename)
{
    //Leser Fila
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


ContourLines::ContourLines()
{
    createContourLines();
}

void ContourLines::init()
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

void ContourLines::draw()
{
   // glBindVertexArray( getMeshComponent()->mVAO );
   // glUniformMatrix4fv( getMeshComponent()->mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
   // glDrawArrays(GL_TRIANGLES, 0, getMeshComponent()->mVertices.size());

    glBindVertexArray( getMeshComponent()->mVAO );
    glDrawArrays(GL_LINES, 0, getMeshComponent()->mVertices.size());
    glBindVertexArray(0);
}

void ContourLines::createContourLines()
{
    float gridSize = 10;
    float terrainWidth = 1000;
    float terrainLength = 1460;
    float xMax = 997;
    float yMax = 95;
    float zMax = 1457;
    float ekv = 5.f;
    int c {0};

   /*  qDebug() << "contourlines size: " << contourLineMesh->mVertices.size()- (terrainWidth/gridSize);
     for(int i = 0; i < contourLineMesh->mVertices.size() - (terrainWidth/gridSize)-1; i++)
     {
        qDebug() << " " << contourLineMesh->mVertices.at(i+(int(terrainWidth/gridSize))+1).getXYZ().getX();
     }*/

   for(int h = {0}; h < yMax; h += ekv)
    {
        for(int i = 0; i < contourLineMesh->mVertices.size() - (terrainWidth/gridSize)-1; i++)
        {

            if (c == (terrainWidth/gridSize) -1)
            {
                c = 0;
                continue;
            }
            gsl::Vector2D p0 {contourLineMesh->mVertices.at(i).getXYZ().getX(), contourLineMesh->mVertices.at(i).getXYZ().getY()};
            gsl::Vector2D p1 {contourLineMesh->mVertices.at(i+1).getXYZ().getX(), contourLineMesh->mVertices.at(i+1).getXYZ().getY()};
            gsl::Vector2D p2 {contourLineMesh->mVertices.at(i+int(terrainWidth/gridSize)).getXYZ().getX(), contourLineMesh->mVertices.at(i+int(terrainWidth/gridSize)).getXYZ().getY()};
            gsl::Vector2D p3 {contourLineMesh->mVertices.at(i+(int(terrainWidth/gridSize))+1).getXYZ().getX(),contourLineMesh->mVertices.at(i+(int(terrainWidth/gridSize))+1).getXYZ().getY()};

        if(p0.y < h && p1.y > h)
        {
            float a = h - p0.y;
            float b = p1.y - p0.y;
            float pros = a/b;
            float posx = pros*gridSize;
            getMeshComponent()->mVertices.push_back(Vertex(contourLineMesh->mVertices.at(i).getXYZ().getX() - posx, h, contourLineMesh->mVertices.at(i).getXYZ().getZ(), 0,1,0,   0,0));
        }
        else if (p0.y > h && p1.y < h)
        {
            float a = h - p1.y;
            float b = p0.y - p1.y;
            float pros = a/b;
            float posx = (1-pros)* gridSize;
            getMeshComponent()->mVertices.push_back(Vertex(contourLineMesh->mVertices.at(i).getXYZ().getX() - posx, h, contourLineMesh->mVertices.at(i).getXYZ().getZ(), 0,1,0,  0,0));
        }

        if(p0.y < h && p2.y > h)
        {
            float a = h - p0.y;
            float b = p2.y - p0.y;
            float pros = a/b;
            float posz = pros*gridSize;
            getMeshComponent()->mVertices.push_back(Vertex(contourLineMesh->mVertices.at(i).getXYZ().getX(), h, contourLineMesh->mVertices.at(i).getXYZ().getZ() + posz, 0,1,0,  0,0));
        }
        else if (p0.y > h && p2.y < h)
        {
            float a = h - p0.y;
            float b = p0.y - p2.y;
            float pros = a/b;
            float posz = (pros)*gridSize;
            getMeshComponent()->mVertices.push_back(Vertex(contourLineMesh->mVertices.at(i).getXYZ().getX(), h, contourLineMesh->mVertices.at(i).getXYZ().getZ() - posz, 0,1,0,  0,0));
        }

        if(p1.y < h && p2.y > h)
        {
            float a = h - p1.y;
            float b = p2.y - p1.y;
            float pros = a/b;
            float posxz = (pros)*gridSize;
            getMeshComponent()->mVertices.push_back(Vertex(contourLineMesh->mVertices.at(i+1).getXYZ().getX() + posxz, h, contourLineMesh->mVertices.at(i+1).getXYZ().getZ() + posxz , 0,1,0,  0,0));
        }
        else if (p1.y > h && p2.y < h)
        {
            float a = h - p1.y;
            float b = p1.y - p2.y;
            float pros = a/b;
            float posxz = (pros)*gridSize;
            getMeshComponent()->mVertices.push_back(Vertex(contourLineMesh->mVertices.at(i+1).getXYZ().getX() - posxz, h, contourLineMesh->mVertices.at(i+1).getXYZ().getZ() - posxz, 0,1,0,  0,0));
        }



        if(p2.y < h && p3.y > h)
        {
            float a = h - p2.y;
            float b = p3.y - p2.y;
            float pros = a/b;
            float posx = pros*gridSize;
           getMeshComponent()->mVertices.push_back(Vertex(contourLineMesh->mVertices.at(i+(terrainWidth/gridSize)).getXYZ().x  - posx, h, contourLineMesh->mVertices.at(i+100).getXYZ().getZ(), 0,1,0,  0,0));
        }
        else if (p2.y > h && p3.y < h)
        {
            float a = h - p3.y;
            float b = p2.y - p3.y;
            float pros = a/b;
            float posx = (1-pros)* gridSize;
           getMeshComponent()->mVertices.push_back(Vertex(contourLineMesh->mVertices.at(i+(terrainWidth/gridSize)).getXYZ().getX()  - posx, h, contourLineMesh->mVertices.at(i+(terrainWidth/gridSize)).getXYZ().getZ(), 0,1,0,  0,0));
        }

        if(p1.y < h && p3.y > h)
        {
            float a = h - p1.y;
            float b = p3.y - p1.y;
            float pros = a/b;
            float posz = pros*gridSize;
            getMeshComponent()->mVertices.push_back(Vertex(contourLineMesh->mVertices.at(i+1).getXYZ().x, h, contourLineMesh->mVertices.at(i+1).getXYZ().getZ() + posz, 0,1,0,  0,0));
        }
        else if (p1.y > h && p3.y < h)
        {
            float a = h - p1.y;
            float b = p1.y - p3.y;
            float pros = a/b;
            float posz = (pros)*gridSize;
            getMeshComponent()->mVertices.push_back(Vertex(contourLineMesh->mVertices.at(i+1).getXYZ().getX(), h, contourLineMesh->mVertices.at(i+1).getXYZ().getZ() - posz, 0,1,0,  0,0));
        }

        if(p1.y < h && p2.y > h)
        {
            float a = h - p1.y;
            float b = p2.y - p1.y;
            float pros = a/b;
            float posxz = (pros)*gridSize;
            getMeshComponent()->mVertices.push_back(Vertex(contourLineMesh->mVertices.at(i+1).getXYZ().getX() + posxz, h, contourLineMesh->mVertices.at(i+1).getXYZ().getZ() + posxz , 0,1,0,  0,0));
        }
        else if (p1.y > h && p2.y < h)
        {
            float a = h - p1.y;
            float b = p1.y - p2.y;
            float pros = a/b;
            float posxz = (pros)*gridSize;
            getMeshComponent()->mVertices.push_back(Vertex(contourLineMesh->mVertices.at(i+1).getXYZ().getX() - posxz, h, contourLineMesh->mVertices.at(i+1).getXYZ().getZ() - posxz, 0,1,0,  0,0));
        }
        c++;
    }

  }
   // contourLineMesh->mDrawType = GL_LINES;
}












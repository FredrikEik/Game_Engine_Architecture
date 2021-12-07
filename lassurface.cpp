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
    const int quadAmountX = (xMax-xMin)/step; //qDebug() << quadAmountX;
    const int quadAmountZ = (zMax-zMin)/step; //qDebug() << quadAmountZ;
    //const int numOfQuads = quadAmountX*quadAmountZ; //qDebug() << numOfQuads;

    // An array of vectors that hold all the heights for each quad
    // Size of array is hardcoded, unsure how to do it from numOfQuads
    //std::array<std::vector<float>,900> heights;
    //std::array<std::vector<float>,numOfQuads> heights;

    //An array that holds the average height of each quad
    //float averageHeights[900];

    std::array<std::array<std::vector<float>,80>,80> heights;
    float averageHeights[80][80];
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

        //Pushes the height(y) to the correct vector in the array
        //heights[vectorIndex].push_back(lasData[i].getXYZ().getY());

        heights[quadCoordX][quadCoordZ].push_back(lasData[i].getXYZ().getY());
    }

    float average = 0;
    for(int i = 0; i < heights.size(); i++)
    {
        for(int j = 0; j < heights[i].size(); j++)
        {
            average = 0;

            if(heights[i][j].size()>0)
            {
                for(int k = 0; k < heights[i][j].size(); k++)
                {
                    average += heights[i][j][k];
                    average = average/heights[i][j].size();
                    averageHeights[i][j] = average;
                }
            }
            else{
                averageHeights[i][j] = 0.f;
            }
        }
    }

    //Create triangulated surface
    float R = 10;
    float G = 20;
    float B = 40;
    for (float z = zMin; z < zMax; z+= step)
    {
        for(float x = xMin; x < xMax; x+= step)
        {
            int quadCoordX = (x-xMin)/step;
            int quadCoordZ = (z-zMin)/step;

            getMeshComponent()
            ->mVertices.push_back(
                Vertex(x, averageHeights[quadCoordX][quadCoordZ], z,
                R/255, averageHeights[quadCoordX][quadCoordZ]*G/255, B/255,
                quadCoordX,quadCoordZ));
        }
    }

    for(int i = 0; i < quadAmountX-1; i++)
    {
        for(int j = 0; j < quadAmountZ-1; j++)
        {
            /*x0z0*/getMeshComponent()->mIndices.push_back(  i+( j   *quadAmountZ)); /*0 */
            /*x0z1*/getMeshComponent()->mIndices.push_back(  i+((j+1)*quadAmountZ)); /*30*/
            /*x1z0*/getMeshComponent()->mIndices.push_back(1+i+( j   *quadAmountZ)); /*1 */
            /*x1z1*/getMeshComponent()->mIndices.push_back(1+i+((j+1)*quadAmountZ)); /*31*/
            /*x1z0*/getMeshComponent()->mIndices.push_back(1+i+( j   *quadAmountZ)); /*1 */
            /*x0z1*/getMeshComponent()->mIndices.push_back(  i+((j+1)*quadAmountZ)); /*30*/
        }
    }
    /* Print all vertices in trianglesurface*/
//    for(int i = 0; i < getMeshComponent()->mVertices.size(); i++)
//    {
//        qDebug() << getMeshComponent()->mVertices[i].getXYZ().getX()
//                 << getMeshComponent()->mVertices[i].getXYZ().getY()
//                 << getMeshComponent()->mVertices[i].getXYZ().getZ();
//    }
    makeContourLines();
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
}

void LASsurface::makeContourLines()
{
    float contourMin = yMin+0.5f;
    float contourMax = yMax-0.5f;
    float contourStep = 0.5f;
    std::vector<Vertex> vertices = getMeshComponent()->mVertices;
    std::vector<GLuint> indices = getMeshComponent()->mIndices;

    for(float contourHeight = contourMin; contourHeight < contourMax; contourHeight+=contourStep)
    {
        //For each quad
        for(int i = 0; i < getMeshComponent()->mIndices.size(); i+=6)
        {
            //a, d, b, c, b, d
            Vertex a = vertices[indices[i]];   bool A = false;
            Vertex b = vertices[indices[i+2]]; bool B = false;
            Vertex c = vertices[indices[i+3]]; bool C = false;
            Vertex d = vertices[indices[i+1]]; bool D = false;
            if(a.getXYZ().getY() > contourHeight){
                A = true;
            }
            if(b.getXYZ().getY() > contourHeight){
                B = true;
            }
            if(c.getXYZ().getY() > contourHeight){
                C = true;
            }
            if(d.getXYZ().getY() > contourHeight){
                D = true;
            }
            Vertex bd = (b+d)/2;   //Meeting point for case 3, 6, 9, 12
            Vertex bbd = (b+bd)/2; //Meeting point for case 4, 10, 11
            Vertex dbd = (d+bd)/2; //Meeting point for case 1, 10, 14
            Vertex ab = (a+b)/2;
            Vertex bc = (b+c)/2;
            Vertex cd = (c+d)/2;
            Vertex da = (d+a)/2;

            ab.set_y(contourHeight);
            bc.set_y(contourHeight);
            cd.set_y(contourHeight);
            da.set_y(contourHeight);

            bd.set_y(contourHeight);
            bbd.set_y(contourHeight);
            dbd.set_y(contourHeight);
            //        Vertex ab = a;
            //        ab.set_x(ab.getXYZ().getX() + (step/2));
            //        ab.set_y(contourHeight);
            //        Vertex bc = b;
            //        bc.set_z(ab.getXYZ().getZ() + (step/2));
            //        bc.set_y(contourHeight);
            //        Vertex cd = c;
            //        cd.set_x(ab.getXYZ().getX() - (step/2));
            //        cd.set_y(contourHeight);
            //        Vertex da = d;
            //        da.set_z(ab.getXYZ().getZ() - (step/2));
            //        da.set_y(contourHeight);

            int state = getState(A,B,C,D);

            switch(state)
            {
            case 0:
                //don't draw line
                break;
            case 1:
                //cd to da
                contourPoints.push_back(cd);
                contourPoints.push_back(dbd);
                contourPoints.push_back(dbd);
                contourPoints.push_back(da);
                break;
            case 2:
                //bc to cd
                contourPoints.push_back(bc);
                contourPoints.push_back(cd);
                break;
            case 3:
                //bc to da
                contourPoints.push_back(bc);
                contourPoints.push_back(bd);
                contourPoints.push_back(bd);
                contourPoints.push_back(da);
                break;
            case 4:
                //ab to bc
                contourPoints.push_back(ab);
                contourPoints.push_back(bbd);
                contourPoints.push_back(bbd);
                contourPoints.push_back(bc);
                break;
            case 5:
                //ab to da
                contourPoints.push_back(ab);
                contourPoints.push_back(da);
                //and
                //bc to cd
                contourPoints.push_back(bc);
                contourPoints.push_back(cd);
                break;
            case 6:
                //ab to cd
                contourPoints.push_back(ab);
                contourPoints.push_back(bd);
                contourPoints.push_back(bd);
                contourPoints.push_back(cd);
                break;
            case 7:
                //ab to da
                contourPoints.push_back(ab);
                contourPoints.push_back(da);
                break;
            case 8:
                //ab to da
                contourPoints.push_back(ab);
                contourPoints.push_back(da);
                break;
            case 9:
                //ab to cd
                contourPoints.push_back(ab);
                contourPoints.push_back(bd);
                contourPoints.push_back(bd);
                contourPoints.push_back(cd);
                break;
            case 10:
                //ab to bc
                contourPoints.push_back(ab);
                contourPoints.push_back(bbd);
                contourPoints.push_back(bbd);
                contourPoints.push_back(bc);
                //and
                //cd to da
                contourPoints.push_back(cd);
                contourPoints.push_back(dbd);
                contourPoints.push_back(dbd);
                contourPoints.push_back(da);
                break;
            case 11:
                //ab to bc
                contourPoints.push_back(ab);
                contourPoints.push_back(bbd);
                contourPoints.push_back(bbd);
                contourPoints.push_back(bc);
                break;
            case 12:
                //bc to da
                contourPoints.push_back(bc);
                contourPoints.push_back(bd);
                contourPoints.push_back(bd);
                contourPoints.push_back(da);
                break;
            case 13:
                //bc to cd
                contourPoints.push_back(bc);
                contourPoints.push_back(cd);
                break;
            case 14:
                //cd to da
                contourPoints.push_back(cd);
                contourPoints.push_back(dbd);
                contourPoints.push_back(dbd);
                contourPoints.push_back(da);
                break;
            case 15:
                //don't draw line
                break;
            }
        }
    }
}

int LASsurface::getState(bool A, bool B, bool C, bool D)
{
    return 8*A+4*B+2*C+1*D;
}

void LASsurface::init()
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

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &getMeshComponent()->mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getMeshComponent()->mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getMeshComponent()->mIndices.size() * sizeof(GLuint), getMeshComponent()->mIndices.data(), GL_STATIC_DRAW);

}

void LASsurface::draw()
{
    glBindVertexArray(getMeshComponent()->mVAO );
    glDrawElements(GL_TRIANGLES, getMeshComponent()->mIndices.size(), GL_UNSIGNED_INT, nullptr);
    //glDrawArrays(GL_LINE_STRIP, 0, getMeshComponent()->mVertices.size());
    glBindVertexArray(0);
}

void LASsurface::move(float x, float y, float z)
{
    getTransformComponent()->mMatrix.translate(x,y,z);
    getSphereCollisionComponent()->center += gsl::Vector3D(x,y,z);
}






#include "lasterraingenerator.h"
#include "gsl/vector4d.h"
#include "QDebug"

LasTerrainGenerator::LasTerrainGenerator()
{
    readFile("../GEA2021/test_las.txt");
    minMaxNormalize();
}
LasTerrainGenerator::~LasTerrainGenerator()
{

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
void LasTerrainGenerator::init()
{
    //getMeshComponent()->mMatrixUniform = matrixUniform;
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
void LasTerrainGenerator::move(float x, float y, float z)
{

}


void LasTerrainGenerator::readFile(std::string filename)
{


    unsigned int n;
    bool once = true;
    int size;
    std::ifstream inn;
    inn.open(filename.c_str());

    if(inn.is_open())
    {
        Vertex pos;
        inn>>n;
        size = n;
        //        float i{0};
        mPointsArray.reserve(n);



        for(int i = 0; i < size; i++)
        {
            inn >> x;
            inn >> z;
            inn >> y;



            pos.set_xyz(x,y,z);


            //getMeshComponent()->mVertices.push_back();
            mPointsArray.push_back(pos);  //push_back(pos);


            if(once)
            {
                lowestX = x;
                highestX = x;
                lowestZ = z;
                highestZ = z;
                lowestY = y;
                highestY = y;
                once = false;
            }
            if(x < lowestX)
                lowestX = x;

            if(x > highestX)
                highestX = x;

            if(z < lowestZ)
                lowestZ = z;

            if(z > highestZ)
                highestZ = z;

            if(y < lowestY)
                lowestY = y;

            if(y > highestY)
                highestY = y;

        }
        inn.close();

        for(unsigned long long i = 0; i < size; i++)
        {
          //mPointsArray[i].setX(mPointsArray[i].getX() - xMin);
          //mPointsArray[i].setZ(mPointsArray[i].getZ() - zMin);
          //mPointsArray[i].setY(mPointsArray[i].getY() - yMin);
        //
        }
    }
    /*
    highestX = highestX-lowestX;
    lowestX = lowestX-lowestX;
    highestZ = highestZ-lowestZ;
    lowestZ = lowestZ-lowestZ;
    highestY = highestY-lowestY;
    lowestY = lowestY-lowestY;
    qDebug() << "Highest X: " << highestX;
    qDebug() << "Lowest X: " << lowestX;
    qDebug() << "Highest Z: " << highestZ;
    qDebug() << "Lowest Z: " << lowestZ;
    qDebug() << "Highest Y: " << highestY;
    qDebug() << "Lowest Y: " << lowestY;

    */

}

void LasTerrainGenerator::minMaxNormalize()
{
    for(int i = 0; i < mPointsArray.size(); i++)
    {
        float nx = xMin+(((mPointsArray[i].getXYZ().getX() - lowestX)*(xMax-xMin) / highestX - lowestX));
        float ny = yMin+(((mPointsArray[i].getXYZ().getY() - lowestY)*(xMax-yMin) / highestY - lowestY));
        float nz = zMin+(((mPointsArray[i].getXYZ().getZ() - lowestZ)*(zMax-zMin) / highestZ - lowestZ));
        mPointsArray[i].set_xyz(nx,ny,nz);

    }
}

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
    //const int amountOfQuadsX = abs(xMax-xMin)/step;
    const int amountOfQuadsZ = abs(zMax-zMin)/step;

    //Et Array med alle høyde verdiene til alle quads
    std::array<std::vector<float>,3600> heights;

    //Et array med gjennomsnittet av høydene
    float averageHeights[3600];

    //For alle vertices..
    for(unsigned long long i = 0; i < mPointsArray.size(); i++)
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
        int vectorIndex = quadZ*amountOfQuadsZ + quadX;

        //push backer høyde kordinatene till rett vector i arrayet
        heights[vectorIndex].push_back(mPointsArray[i].getXYZ().getY());
    }

    for(unsigned long long i = 0; i < heights.size(); i++)
    {
        //Calculate the average of all heights in quad
        //.. and put it in the array of averageHeights
        float sum = 0;
        for(unsigned long long j = 0; j < heights[i].size(); j++)
        {
            sum += heights[i][j];
        }
        sum = sum/heights[i].size();
        averageHeights[i] = sum;
    }

    //Lager triangulert overflate her
    float R = 1, G = 1, B = 1; //Må ha rgb

    for (int x = xMin; x < xMax-step; x+= step)
    {
        for(int z = zMin; z < zMax; z+= step)
        {
            int quadX = (x-xMin)/step;
            int quadZ = (z-zMin)/step;
            float u = 0;//{(x + abs(xMin)) / (xMax + abs(xMin) + step)};
            float v = 0;//{(z + abs(zMin)) / (zMax + abs(zMin) + step)};

            getMeshComponent()->mVertices.push_back(Vertex(x, averageHeights[quadZ*amountOfQuadsZ + quadX], z, R / 255, averageHeights[quadZ*amountOfQuadsZ + quadX]*G/255, B/255,u,v));
            getMeshComponent()->mVertices.push_back(Vertex(x, averageHeights[(quadZ+1)*amountOfQuadsZ + quadX], z+step,R/255, averageHeights[(quadZ+1)*amountOfQuadsZ + quadX]*G/255, B/255, u, v+step));
            getMeshComponent()->mVertices.push_back(Vertex(x+step, averageHeights[quadZ*amountOfQuadsZ + quadX+1], z,R/255, averageHeights[quadZ*amountOfQuadsZ + quadX+1]*G/255, B/255, u+step,v));
            getMeshComponent()->mVertices.push_back(Vertex(x+step, averageHeights[(quadZ+1)*amountOfQuadsZ + quadX+1], z+step, R/255, averageHeights[(quadZ+1)*amountOfQuadsZ + quadX+1]*G/255, B/255, u+step, v+step));
            getMeshComponent()->mVertices.push_back(Vertex(x+step, averageHeights[quadZ*amountOfQuadsZ + quadX+1], z,R/255, averageHeights[quadZ*amountOfQuadsZ + quadX+1]*G/255, B/255, u+step,v));
            getMeshComponent()->mVertices.push_back(Vertex(x, averageHeights[(quadZ+1)*amountOfQuadsZ + quadX], z+step,R/255, averageHeights[(quadZ+1)*amountOfQuadsZ + quadX]*G/255, B/255, u, v+step));
        }
    }
    contourLineMesh = getMeshComponent();
}
LasTerrainGenerator::~LasTerrainGenerator()
{}
void LasTerrainGenerator::readFile(std::string fileName)
{
    //Leser Fila
    std::ifstream inn;
    inn.open(fileName.c_str());
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
    for(unsigned long long i = 0; i < mPointsArray.size(); i++)
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
    qDebug() << "Making ContourLines!";
    float min = -10.5f;
    float max = 20.5f;
    float step = 0.5f;
    //int A,B,C,D = 0;
    for(auto height = min; height<max; height+=step)
    {
        for(auto i = 0; i < contourLineMesh->mVertices.size(); i += 6)
        {
            Vertex a = contourLineMesh->mVertices [i];
            Vertex b = contourLineMesh->mVertices [i+2];
            Vertex c = contourLineMesh->mVertices [i+3];
            Vertex d = contourLineMesh->mVertices [i+1];
            bool TOPLEFT = false;
            bool TOPRIGHT = false;
            bool BOTTOMLEFT = false;
            bool BOTTOMRIGHT = false;
            if(a.getXYZ().y > height)
            {
                TOPLEFT = 1;
            }
            if(b.getXYZ().y > height)
            {
                TOPRIGHT = 1;
            }
            if(c.getXYZ().y > height)
            {
                BOTTOMRIGHT = 1;
            }
            if(d.getXYZ().y > height)
            {
                BOTTOMLEFT = 1;
            }
            Vertex ab = (a+b)/2; ab.set_y(height);
            Vertex bc = (b+c)/2; bc.set_y(height);
            Vertex cd = (c+d)/2; cd.set_y(height);
            Vertex da = (d+a)/2; da.set_y(height);
            Vertex bd = (b+d)/2; bd.set_y(height);
            Vertex bbd = (b+bd)/2; bbd.set_y(height);
            Vertex dbd = (d+bd)/2; dbd.set_y(height);

            int marchingsquaresValue = 8 * TOPLEFT + 4 * TOPRIGHT + 2 * BOTTOMRIGHT + 1 * BOTTOMLEFT;
            //qDebug() << "Marching squares value: " << marchingsquaresValue;

            switch(marchingsquaresValue)
            {
            //Topleft = a,
            //topright = b,
            //bottomRIGHT = c,
            //bottomLEFT = d.
            case 0:
                //ingenting å pushe
                break;
            case 1:
                            //nederst høyre - nederst venstre til øverst venstre (cd-da)
                            contourLineVertexData.push_back(cd);
                            contourLineVertexData.push_back(dbd);
                            contourLineVertexData.push_back(dbd);
                            contourLineVertexData.push_back(da);
                            break;
                        case 2:
                            //øverst høyre - nederst høyre til nederst venstre (bc to cd)
                            contourLineVertexData.push_back(bc);
                            contourLineVertexData.push_back(cd);
                            break;
                        case 3:
                            //øverst høyre - nederst høyre til nederst høre - øverst venstre (bc to da)
                            contourLineVertexData.push_back(bc);
                            contourLineVertexData.push_back(bd);
                            contourLineVertexData.push_back(bd);
                            contourLineVertexData.push_back(da);
                            break;
                        case 4:
                            //øverst venstre - øverst høyre til nederst høyre (ab to bc)
                            contourLineVertexData.push_back(ab);
                            contourLineVertexData.push_back(bbd);
                            contourLineVertexData.push_back(bbd);
                            contourLineVertexData.push_back(bc);
                            break;
                        case 5:
                            //øverst venstre - øverst høyre til (ab to da)
                            contourLineVertexData.push_back(ab);
                            contourLineVertexData.push_back(da);
                            //and
                            //øverst høyre - nederst høyre til øverst høyre - nederst venstre (bc to cd)
                            contourLineVertexData.push_back(bc);
                            contourLineVertexData.push_back(cd);
                            break;
                        case 6:
                            //øverst venstre - øverst høyre til nederst høyre - nederst venstre (ab to cd)
                            contourLineVertexData.push_back(ab);
                            contourLineVertexData.push_back(bd);
                            contourLineVertexData.push_back(bd);
                            contourLineVertexData.push_back(cd);
                            break;
                        case 7:
                            //øverst venstre - øverst høyre til nederst venstre - øverst venstre(ab to da)
                            contourLineVertexData.push_back(ab);
                            contourLineVertexData.push_back(da);
                            break;
                        case 8:
                            //øverst venstre - øverst høyre til nedest høyre il øverst venstre(ab to da)
                            contourLineVertexData.push_back(ab);
                            contourLineVertexData.push_back(da);
                            break;
                        case 9:
                            //øverst venstre - øverst høyre til nederst høyre - nederst venstre (ab to cd)
                            contourLineVertexData.push_back(ab);
                            contourLineVertexData.push_back(bd);
                            contourLineVertexData.push_back(bd);
                            contourLineVertexData.push_back(cd);
                            break;
                        case 10:
                            //øverst venstre - øvert høyre til øverst(ab to bc)
                            contourLineVertexData.push_back(ab);
                            contourLineVertexData.push_back(bbd);
                            contourLineVertexData.push_back(bbd);
                            contourLineVertexData.push_back(bc);
                            //and
                            //nederst høyre - nederst venstre til nederst venstre - øverst høyre (cd to da)
                            contourLineVertexData.push_back(cd);
                            contourLineVertexData.push_back(dbd);
                            contourLineVertexData.push_back(dbd);
                            contourLineVertexData.push_back(da);
                            break;
                        case 11:
                            //øverst venstre - øverst høyre til øverst høyre - nedest høyre(ab to bc)
                            contourLineVertexData.push_back(ab);
                            contourLineVertexData.push_back(bbd);
                            contourLineVertexData.push_back(bbd);
                            contourLineVertexData.push_back(bc);
                            break;
                        case 12:
                            //øverst høyre - nederst høyre til nedest venstre til øverst venstre(bc to da)
                            contourLineVertexData.push_back(bc);
                            contourLineVertexData.push_back(bd);
                            contourLineVertexData.push_back(bd);
                            contourLineVertexData.push_back(da);
                            break;
                        case 13:
                            //øverst høyre - nederst høyre til nederst høyre - nederst venstre (bc to cd)
                            contourLineVertexData.push_back(bc);
                            contourLineVertexData.push_back(cd);
                            break;
                        case 14:
                            //nederst høyre - nederst venstre til nederst venstre - øverst venstre (cd to da)
                            contourLineVertexData.push_back(cd);
                            contourLineVertexData.push_back(dbd);
                            contourLineVertexData.push_back(dbd);
                            contourLineVertexData.push_back(da);
                            break;
                case 15:
                {
                    //ingenting å pushe
                    break;
                }
            }
        }
    }
    getMeshComponent()->mVertices = contourLineVertexData;
}









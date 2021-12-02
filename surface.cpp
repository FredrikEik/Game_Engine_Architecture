#include "surface.h"
#include "vertex.h"
#include "MathStuff/MathStuff.h"

#include "components.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <math.h>

//MeshComponent* contourLineMesh;

Surface::Surface(std::string filename) : GameObject() //går her når filepath er inputta i renderwindow.cpp new Surface.
{
    readFile(filename);
    //mMatrix.setToIdentity();
    getTransformComp()->mMatrix.setToIdentity();
    //contourLineMesh = getMeshComp();
}

Surface::~Surface()
{

}

void Surface::readFile(std::string filename)
{
    //Open File
    //    std::string filename = Orf::assetFilePath.toStdString() + fileName + ".obj";
    std::ifstream fileIn;
    fileIn.open (filename, std::ifstream::in);
    if(!fileIn)
        qDebug() << "Could not open file for reading: " << QString::fromStdString(filename);

    //One line at a time-variable
    std::string oneLine;
    //One word at a time-variable
    std::string oneWord;

    std::vector<gsl::Vector3D> tempVertecies;
    std::vector<gsl::Vector3D> tempNormals;
    std::vector<gsl::Vector2D> tempUVs;

    std::getline(fileIn, oneLine);
    std::stringstream sStream;
    sStream << oneLine;
    sStream >> oneWord;
    int num = std::atoi(oneWord.c_str());
    //qDebug() << "Vertices: " << num;

    num = 25000;
    unsigned int temp_index = 0;

    for(int i = 0; i < num; i++)
    {
        std::getline(fileIn, oneLine);
        std::stringstream sStream;
        //Pushing line into stream
        sStream << oneLine;
        //Streaming one word out of line
        oneWord = ""; //resetting the value or else the last value might survive!

        gsl::Vector3D tempVertex;
        sStream >> oneWord;
        tempVertex.x = std::stof(oneWord); // - 615200.f
        sStream >> oneWord;
        tempVertex.y = std::stof(oneWord); // - 6758325.f
        sStream >> oneWord;
        tempVertex.z = std::stof(oneWord); // - 565.f
        //qDebug() << "x: " << tempVertex.x << " y: " << tempVertex.y << " z: " << tempVertex.z;

        //Vertex made - pushing it into vertex-vector
        tempVertecies.push_back(tempVertex);

        Vertex tempVert(tempVertex, gsl::Vector3D(1.f, 0.f, 0.f), gsl::Vector2D(0.f, 0.f));
        /*if(getMeshComp())
        {
            getMeshComp()->mVertices.push_back(tempVert);
            getMeshComp()->mIndices.push_back(temp_index++);
        }*/
    }

    gsl::Vector3D minPointPosition = tempVertecies[0];
    gsl::Vector3D maxPointPosition = tempVertecies[0];

    for( int i = 0; i < num; i++)
    {
        minPointPosition.x = tog::min(minPointPosition.x, tempVertecies[i].x); //finds the lowest xyz
        minPointPosition.y = tog::min(minPointPosition.y, tempVertecies[i].y);
        minPointPosition.z = tog::min(minPointPosition.z, tempVertecies[i].z);
        maxPointPosition.x = tog::max(maxPointPosition.x, tempVertecies[i].x); //finds the highest xyz
        maxPointPosition.y = tog::max(maxPointPosition.y, tempVertecies[i].y);
        maxPointPosition.z = tog::max(maxPointPosition.z, tempVertecies[i].z);
    }
    //qDebug() << "minX: " << minPointPosition.x << " minY: " << minPointPosition.y << " minZ: " << minPointPosition.z;
    //qDebug() << "maxX: " << maxPointPosition.x << " maxY: " << maxPointPosition.y << " maxZ: " << maxPointPosition.z;

    gsl::Vector3D tempOffset = -minPointPosition;

    for( int i = 0; i < num; i++)
    {
        tempVertecies[i] += tempOffset; // putting the points closer to origin
        //qDebug() << "x: " << tempVertecies[i].x << " y: " << tempVertecies[i].y << " z: " << tempVertecies[i].z;
    }
    minPointPosition += tempOffset;
    maxPointPosition += tempOffset;

    static constexpr unsigned triangleNumberX = 50; // sqares in x direction
    static constexpr unsigned triangleNumberY = 50; // sqares in y direction
    const gsl::Vector2D distance = gsl::Vector2D((maxPointPosition.x - minPointPosition.x) / triangleNumberX, (maxPointPosition.y - minPointPosition.y) / triangleNumberY);
    //const gsl::Vector2D distance = gsl::Vector2D(maxPointPosition.x - minPointPosition.x / triangleNumberX, maxPointPosition.y - minPointPosition.y / triangleNumberY);
    gsl::Vector3D tempVector;
    int closestVert;
    float vertDistance;
    float closestVertDistance;

    getMeshComp()->mVertices.resize(triangleNumberX * triangleNumberY);
    auto verticesVectorGetIndex = [](unsigned x, unsigned y) -> unsigned {return x * triangleNumberY + y; };

    for (unsigned x = 0; x < triangleNumberX; x++)
    {
        for (unsigned y = 0; y < triangleNumberY; y++)
        {
            tempVector = gsl::Vector3D(x * distance.x, y * distance.y, 0.f);

            closestVertDistance = tog::distanceVec3D(tempVector, gsl::Vector3D(tempVertecies[0].x, tempVertecies[0].y, 0.f));
            closestVert = 0;

            for(unsigned i = 0; i < num; i++) // finds closest point to perfect square corner
            {
                vertDistance = tog::distanceVec3D(tempVector, gsl::Vector3D(tempVertecies[i].x, tempVertecies[i].y, 0.f));

                if (vertDistance < closestVertDistance)
                {
                    closestVert = i;
                    closestVertDistance = vertDistance;
                }
            }
            getMeshComp()->mVertices[verticesVectorGetIndex(x, y)] = Vertex(gsl::Vector3D(tempVertecies[closestVert].x, tempVertecies[closestVert].z, tempVertecies[closestVert].y),
                                                                            gsl::Vector3D(1.f, 0.f, 0.f),
                                                                            gsl::Vector2D((rand()%101)/10, (rand()%101)/10));
        }
    }

    for (unsigned i = 0; i < getMeshComp()->mVertices.size(); i++)
    {
        qDebug() << "x: " << getMeshComp()->mVertices[i].get_xyz().x << " y: " << getMeshComp()->mVertices[i].get_xyz().y << " z: " << getMeshComp()->mVertices[i].get_xyz().z;
    }

    getMeshComp()->mIndices.reserve(triangleNumberX * triangleNumberY * 6);

    for (unsigned x = 0; x < triangleNumberX - 1; x++) // assing indexes for the sqares
    {
        for (unsigned y = 0; y < triangleNumberY - 1; y++)
        {
            getMeshComp()->mIndices.push_back(verticesVectorGetIndex(x, y));
            getMeshComp()->mIndices.push_back(verticesVectorGetIndex(x, y + 1));
            getMeshComp()->mIndices.push_back(verticesVectorGetIndex(x + 1, y));

            getMeshComp()->mIndices.push_back(verticesVectorGetIndex(x + 1, y));
            getMeshComp()->mIndices.push_back(verticesVectorGetIndex(x, y + 1));
            getMeshComp()->mIndices.push_back(verticesVectorGetIndex(x + 1, y + 1));

        }
    }

    //closing the file after use
    fileIn.close();

}

void Surface::init()
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &getMeshComp()->mVAO );
    glBindVertexArray( getMeshComp()->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &getMeshComp()->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, getMeshComp()->mVBO );

    glBufferData( GL_ARRAY_BUFFER, getMeshComp()->mVertices.size()*sizeof(Vertex), getMeshComp()->mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)( 6 * sizeof( GLfloat ) ));
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &getMeshComp()->mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getMeshComp()->mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getMeshComp()->mIndices.size() * sizeof(GLuint), getMeshComp()->mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Surface::draw()
{
    glBindVertexArray( getMeshComp()->mVAO );
    glDrawElements(GL_TRIANGLES, getMeshComp()->mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

#include "planeimport.h"
#include "vertex.h"

#include "components.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

PlaneImport::PlaneImport(std::string filename) : GameObject()
{
    readFile(filename);
    getTransformComp()->mMatrix.setToIdentity();
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

void PlaneImport::draw()
{
    glBindVertexArray( getMeshComp()->mVAO );
    glDrawElements(GL_TRIANGLES, getMeshComp()->mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void PlaneImport::readFile(std::string filename)
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
    qDebug() << "Vertices: " << num;

    for(int i = 0; i < num; i++)
    {
        std::getline(fileIn, oneLine);
        std::stringstream sStream;
        //Pushing line into stream
        sStream << oneLine;
        //Streaming one word out of line
        oneWord = ""; //resetting the value or else the last value might survive!
        sStream >> oneWord;

        gsl::Vector3D tempVertex;
        sStream >> oneWord;
        tempVertex.x = std::stof(oneWord);
        sStream >> oneWord;
        tempVertex.y = std::stof(oneWord);
        sStream >> oneWord;
        tempVertex.z = std::stof(oneWord);

        //Vertex made - pushing it into vertex-vector
        tempVertecies.push_back(tempVertex);

        Vertex tempVert(tempVertex, gsl::Vector3D(0.f, 0.f, 0.f), gsl::Vector2D(0.f, 0.f));
        //getMeshComp()->mVertices.push_back(tempVertex);
    }

    //closing the file after use
    fileIn.close();
}

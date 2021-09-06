#include "objmesh.h"
#include "vector3d.h"
#include "vector2d.h"
#include "vertex.h"
#include <vector>
#include "systems/ecs/Components.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

ObjMesh::ObjMesh(std::string filename) : VisualObject ()
{
    mComponents.push_back(new TransformComponent());
   TransformComponent *tempTrans = static_cast<TransformComponent*>(mComponents.back());
    tempTrans->mMatrix.setToIdentity();
    readFile(filename);

    //qDebug() << filename.data();
}

ObjMesh::~ObjMesh()
{
}

void ObjMesh::init()
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    MeshComponent *tempMesh = static_cast<MeshComponent*>(mComponents.at(1));

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &tempMesh->mVAO );
    glBindVertexArray( tempMesh->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &tempMesh->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, tempMesh->mVBO );

    glBufferData( GL_ARRAY_BUFFER, tempMesh->mVertices.size()*sizeof(Vertex), tempMesh->mVertices.data(), GL_STATIC_DRAW );

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
    glGenBuffers(1, &tempMesh->mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tempMesh->mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, tempMesh->mIndices.size() * sizeof(GLuint), tempMesh->mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void ObjMesh::readFile(std::string filename)
{
    mComponents.push_back(new MeshComponent());
    MeshComponent *tempMesh = static_cast<MeshComponent*>(mComponents.at(1));
    //Open File
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

    //    std::vector<Vertex> mVertices;    //made in VisualObject
    //    std::vector<GLushort> mIndices;   //made in VisualObject

    // Varible for constructing the indices vector
    unsigned int temp_index = 0;

    //Reading one line at a time from file to oneLine
    while(std::getline(fileIn, oneLine))
    {
        //Doing a trick to get one word at a time
        std::stringstream sStream;
        //Pushing line into stream
        sStream << oneLine;
        //Streaming one word out of line
        oneWord = ""; //resetting the value or else the last value might survive!
        sStream >> oneWord;

        if (oneWord == "#")
        {
            //Ignore this line
            //            qDebug() << "Line is comment "  << QString::fromStdString(oneWord);
            continue;
        }
        if (oneWord == "")
        {
            //Ignore this line
            //            qDebug() << "Line is blank ";
            continue;
        }
        if (oneWord == "v")
        {
            //            qDebug() << "Line is vertex "  << QString::fromStdString(oneWord) << " ";
            gsl::Vector3D tempVertex;
            sStream >> oneWord;
            tempVertex.x = std::stof(oneWord);
            sStream >> oneWord;
            tempVertex.y = std::stof(oneWord);
            sStream >> oneWord;
            tempVertex.z = std::stof(oneWord);

            //Vertex made - pushing it into vertex-vector
            tempVertecies.push_back(tempVertex);

            continue;
        }
        if (oneWord == "vt")
        {
            //            qDebug() << "Line is UV-coordinate "  << QString::fromStdString(oneWord) << " ";
            gsl::Vector2D tempUV;
            sStream >> oneWord;
            tempUV.x = std::stof(oneWord);
            sStream >> oneWord;
            tempUV.y = std::stof(oneWord);

            //UV made - pushing it into UV-vector
            tempUVs.push_back(tempUV);

            continue;
        }
        if (oneWord == "vn")
        {
            //            qDebug() << "Line is normal "  << QString::fromStdString(oneWord) << " ";
            gsl::Vector3D tempNormal;
            sStream >> oneWord;
            tempNormal.x = std::stof(oneWord);
            sStream >> oneWord;
            tempNormal.y = std::stof(oneWord);
            sStream >> oneWord;
            tempNormal.z = std::stof(oneWord);

            //Vertex made - pushing it into vertex-vector
            tempNormals.push_back(tempNormal);
            continue;
        }
        if (oneWord == "f")
        {
            //            qDebug() << "Line is a face "  << QString::fromStdString(oneWord) << " ";
            //int slash; //used to get the / from the v/t/n - format
            int index, normal, uv;
            for(int i = 0; i < 3; i++)
            {
                sStream >> oneWord;     //one word read
                std::stringstream tempWord(oneWord);    //to use getline on this one word
                std::string segment;    //the numbers in the f-line
                std::vector<std::string> segmentArray;  //temp array of the numbers
                while(std::getline(tempWord, segment, '/')) //splitting word in segments
                {
                    segmentArray.push_back(segment);
                }
                index = std::stoi(segmentArray[0]);     //first is vertex
                if (segmentArray[1] != "")              //second is uv
                    uv = std::stoi(segmentArray[1]);
                else
                {
                    //qDebug() << "No uvs in mesh";       //uv not present
                    uv = 0;                             //this will become -1 in a couple of lines
                }
                normal = std::stoi(segmentArray[2]);    //third is normal

                //Fixing the indexes
                //because obj f-lines starts with 1, not 0
                --index;
                --uv;
                --normal;

                if (uv > -1)    //uv present!
                {
                    Vertex tempVert(tempVertecies[index], tempNormals[normal], tempUVs[uv]);
                    tempMesh->mVertices.push_back(tempVert);
                }
                else            //no uv in mesh data, use 0, 0 as uv
                {
                    Vertex tempVert(tempVertecies[index], tempNormals[normal], gsl::Vector2D(0.0f, 0.0f));
                   tempMesh-> mVertices.push_back(tempVert);
                }
               tempMesh-> mIndices.push_back(temp_index++);
            }
            continue;
        }
    }

    //closing the file after use
    fileIn.close();
}


void ObjMesh::makeVertex()
{

    mComponents.push_back(new MeshComponent());
    MeshComponent *tempMesh = static_cast<MeshComponent*>(mComponents.at(1));
    while(true)
    {
        if(oneLine[OBJi] != '/' && oneLine[OBJi] != ' ' ) {oneWord += oneLine[OBJi]; OBJi++;}
        else {indexPos = std::stoi(oneWord); oneWord.clear(); OBJi++; break;}
    }

    while(true)
    {
        if(oneLine[OBJi] != '/' && oneLine[OBJi] != ' ') {oneWord += oneLine[OBJi]; OBJi++;}
        else {indexUVs = std::stoi(oneWord); oneWord.clear(); OBJi++; break;}
    }

    while(true)
    {
        if(oneLine[OBJi] != ' ' && oneLine[OBJi] != ' ') {oneWord += oneLine[OBJi]; OBJi++;}
        else {indexNorms = std::stoi(oneWord); oneWord.clear(); OBJi++; break;}
    }

    int posIndex = indexPos -1;
    int normalsIndex = indexNorms -1;
    int texcoordsIndex = indexUVs -1;

    tempMesh->mVertices.push_back
            (
                Vertex
                (
                    importedPos[posIndex].x(),
                    importedPos[posIndex].y(),
                    importedPos[posIndex].z(),
                    importedNorms[normalsIndex].x(),
                    importedNorms[normalsIndex].y(),
                    importedNorms[normalsIndex].z(),
                    importedUVs[texcoordsIndex].x(),
                    importedUVs[texcoordsIndex].y()
                )
            );

    tempMesh->mIndices.push_back(indices++);
}

void ObjMesh::draw()
{
    MeshComponent *tempMesh = static_cast<MeshComponent*>(mComponents.at(1));

    glBindVertexArray( tempMesh->mVAO );
    glDrawElements(GL_TRIANGLES, tempMesh->mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
   // qDebug() << "draw obj";
}

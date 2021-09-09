#include "resourcemanager.h"

#include <sstream>
#include <fstream>
#include <vector>
#include <QString>
#include <QDebug>

#include "components.h"
#include "vector3d.h"
#include "vector2d.h"
#include "vertex.h"
#include "gameobject.h"
#include "constants.h"

ResourceManager::ResourceManager()
{
    mMeshComponents.reserve(gsl::MAX_MESHCOMPONENTS);
}

ResourceManager &ResourceManager::getInstance()
{
    static ResourceManager *mInstance = new ResourceManager();
    return *mInstance;
}

GameObject *ResourceManager::AddObject(std::string filename)
{
    int meshIndex{-1};

    //Simple "factory" - making the meshobject said in the filename
    if (filename.find(".obj") != std::string::npos)
        meshIndex = readObj(filename);
    if (filename.find("axis") != std::string::npos)
        meshIndex = makeAxis();
    if (filename.find("triangle") != std::string::npos)
        meshIndex = makeTriangle();

    //safety - if nothing is made I just make a triangle
    if (meshIndex == -1)
        meshIndex = makeTriangle();

    GameObject* tempObject = new GameObject();

    //Dangerous, because vector can resize and will move pointers:
    tempObject->mMesh = &mMeshComponents.at(meshIndex);     //This is a hack, please fix
    tempObject->mMaterial = new MaterialComponent();
    tempObject->mTransform = new TransformComponent();
    tempObject->mTransform->mMatrix.identity();
    return tempObject; //temporary to get to compile
}


int ResourceManager::readObj(std::string filename)
{
    //should check if this object is new before this!
    mMeshComponents.emplace_back(MeshComponent());
    MeshComponent &temp = mMeshComponents.back();

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
                    temp.mVertices.push_back(tempVert);
                }
                else            //no uv in mesh data, use 0, 0 as uv
                {
                    Vertex tempVert(tempVertecies[index], tempNormals[normal], gsl::Vector2D(0.0f, 0.0f));
                    temp.mVertices.push_back(tempVert);
                }
                temp.mIndices.push_back(temp_index++);
            }
            continue;
        }
    }

    //beeing a nice boy and closing the file after use
    fileIn.close();

    initMesh(temp);

    return mMeshComponents.size()-1;    //returns index to last object
}

int ResourceManager::makeAxis()
{
    //should check if this object is new before this!
    mMeshComponents.emplace_back(MeshComponent());
    MeshComponent &temp = mMeshComponents.back();

    temp.mVertices.push_back(Vertex{0.f, 0.f, 0.f, 1.f, 0.f, 0.f});
    temp.mVertices.push_back(Vertex{100.f, 0.f, 0.f, 1.f, 0.f, 0.f});
    temp.mVertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 1.f, 0.f});
    temp.mVertices.push_back(Vertex{0.f, 100.f, 0.f, 0.f, 1.f, 0.f});
    temp.mVertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 0.f, 1.f});
    temp.mVertices.push_back(Vertex{0.f, 0.f, 100.f, 0.f, 0.f, 1.f});

    temp.mDrawType = GL_LINES;

    initMesh(temp);

    return mMeshComponents.size()-1;    //returns index to last object
}

int ResourceManager::makeTriangle()
{
    //should check if this object is new before this!
    mMeshComponents.emplace_back(MeshComponent());
    MeshComponent &temp = mMeshComponents.back();
    temp.mVertices.push_back(Vertex{-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  0.f, 0.f}); // Bottom Left
    temp.mVertices.push_back(Vertex{0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.f}); // Bottom Right
    temp.mVertices.push_back(Vertex{0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.f}); // Top

    temp.mDrawType = GL_TRIANGLES;
    initMesh(temp);

    return mMeshComponents.size()-1;    //returns index to last object
}


void ResourceManager::initMesh(MeshComponent &tempMeshComp)
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &tempMeshComp.mVAO );
    glBindVertexArray( tempMeshComp.mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &tempMeshComp.mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, tempMeshComp.mVBO );

    glBufferData( GL_ARRAY_BUFFER, tempMeshComp.mVertices.size()*sizeof(Vertex), tempMeshComp.mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : position
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)( 6 * sizeof( GLfloat ) ));
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    if(tempMeshComp.mIndices.size() > 0) {
        glGenBuffers(1, &tempMeshComp.mEAB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tempMeshComp.mEAB);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, tempMeshComp.mIndices.size() * sizeof(GLuint), tempMeshComp.mIndices.data(), GL_STATIC_DRAW);
    }

    glBindVertexArray(0);

    tempMeshComp.mIndexCount = tempMeshComp.mIndices.size();
    tempMeshComp.mVertexCount = tempMeshComp.mVertices.size();
}

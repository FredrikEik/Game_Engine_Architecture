#include "resourcemanager.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <qvector3d.h>
#include <qvector2d.h>
#include <vertex.h>
#include "gameobject.h"
#include <QOpenGLFunctions>

ResourceManager::ResourceManager()
{
}

void ResourceManager::readObj(std::string &filename, MeshComponent *MeshComp)
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

    std::vector<QVector3D> tempVertecies;
    std::vector<QVector3D> tempNormals;
    std::vector<QVector2D> tempUVs;

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
            QVector3D tempVertex;
            sStream >> oneWord;
            tempVertex.setX(std::stof(oneWord));
            sStream >> oneWord;
            tempVertex.setY(std::stof(oneWord));
            sStream >> oneWord;
            tempVertex.setZ(std::stof(oneWord));

            //Vertex made - pushing it into vertex-vector
            tempVertecies.push_back(tempVertex);

            continue;
        }
        if (oneWord == "vt")
        {
            //            qDebug() << "Line is UV-coordinate "  << QString::fromStdString(oneWord) << " ";
            QVector2D tempUV;
            sStream >> oneWord;
            tempUV.setX(std::stof(oneWord));
            sStream >> oneWord;
            tempUV.setY(std::stof(oneWord));

            //UV made - pushing it into UV-vector
            tempUVs.push_back(tempUV);

            continue;
        }
        if (oneWord == "vn")
        {
            //            qDebug() << "Line is normal "  << QString::fromStdString(oneWord) << " ";
            QVector3D tempNormal;
            sStream >> oneWord;
            tempNormal.setX(std::stof(oneWord));
            sStream >> oneWord;
            tempNormal.setY(std::stof(oneWord));
            sStream >> oneWord;
            tempNormal.setZ(std::stof(oneWord));

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
                    MeshComp->mVertices.push_back(tempVert);
                }
                else            //no uv in mesh data, use 0, 0 as uv
                {
                    Vertex tempVert(tempVertecies[index], tempNormals[normal], QVector2D(0.0f, 0.0f));
                    MeshComp->mVertices.push_back(tempVert);
                }
                MeshComp->mIndices.push_back(temp_index++);
            }
            continue;
        }
    }
    //beeing a nice boy and closing the file after use
    fileIn.close();
}

void ResourceManager::init(MeshComponent &MeshComp)
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &MeshComp.mVAO );
    glBindVertexArray( MeshComp.mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &MeshComp.mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, MeshComp.mVBO );

    glBufferData( GL_ARRAY_BUFFER, MeshComp.mVertices.size()*sizeof(Vertex), MeshComp.mVertices.data(), GL_STATIC_DRAW );

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
    if(MeshComp.mIndices.size() > 0) {
        glGenBuffers(1, &MeshComp.mEAB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MeshComp.mEAB);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, MeshComp.mIndices.size() * sizeof(GLuint), MeshComp.mIndices.data(), GL_STATIC_DRAW);
    }

    glBindVertexArray(0);
}

GameObject* ResourceManager::CreateObject(std::string filepath)
{
//    if(!(filepath.find(".obj") != std::string::npos))
//    {
        GameObject* temp = new GameObject();
        temp->mMeshComp = new MeshComponent();
        temp->mTransformComp = new TransformComponent();
        temp->mTransformComp->mMatrix.setToIdentity();
        temp->mMaterialComp = new MaterialComponent();

        //Burde ikke hardkode dette
        temp->mMaterialComp->mShaderProgram = 1;
        temp->mMaterialComp->mTextureUnit = 1;

        readObj(filepath, temp->mMeshComp);
        ResourceManager::init(*temp->mMeshComp);
        return temp;
//    }

}






//VisualObject ResourceManager::MakeObject(MeshComponent &meshComp, MaterialComponent &material,
//                                         TransformComponent &transComp)
//{

//}

//VisualObject ResourceManager::MakeObject(bool mesh, bool material, bool transform)
//{
//    VisualObject *temp =  new
//}
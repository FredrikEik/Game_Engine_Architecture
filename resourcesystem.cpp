#include "resourcesystem.h"

resourceSystem::resourceSystem()
{
    std::vector<Vertex> mVertices;
    mVertices.push_back(Vertex{0.f, 0.f, 0.f, 1.f, 0.f, 0.f});
    mVertices.push_back(Vertex{100.f, 0.f, 0.f, 1.f, 0.f, 0.f});
    mVertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 1.f, 0.f});
    mVertices.push_back(Vertex{0.f, 100.f, 0.f, 0.f, 1.f, 0.f});
    mVertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 0.f, 1.f});
    mVertices.push_back(Vertex{0.f, 0.f, 100.f, 0.f, 0.f, 1.f});


    meshContainer.push_back(std::make_pair("XYZ", mVertices));
}

void resourceSystem::CreateMeshComponent(std::string input, MeshComponent * mesh)
{
    meshData obj;
    bool matchFound =false;
    for(auto it = meshDataContainer.begin(); it < meshDataContainer.end();it++)
    {
        if(input == it->first){
            matchFound = true;
            break;
        }
    }

    if(!matchFound){ // Hvis ingen match er funnet, vi lager et nytt object.
        std::string filename = gsl::ModelFilePath + input;
        std::vector<Vertex> tempMVertices;
        std::vector<GLuint> tempMIndices;
        float tempRadius = 0;
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
        std::vector<QVector3D> tempColors;

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
                tempVertex.setY( std::stof(oneWord));
                sStream >> oneWord;
                tempVertex.setZ (std::stof(oneWord));

                //Vertex made - pushing it into vertex-vector
                tempVertecies.push_back(tempVertex);

                //calculate radius of sphere
                tempRadius = calculateLenght(tempVertex);
                //if last radius is less than new radius, apply new radius
                if(mesh->collisionRadius < tempRadius)
                    mesh->collisionRadius = tempRadius;

                continue;
            }
            if (oneWord == "vt")
            {
                //            qDebug() << "Line is UV-coordinate "  << QString::fromStdString(oneWord) << " ";
                QVector2D tempUV;
                sStream >> oneWord;
                tempUV.setX( std::stof(oneWord));
                sStream >> oneWord;
                tempUV.setY( std::stof(oneWord));

                //UV made - pushing it into UV-vector
                tempUVs.push_back(tempUV);

                continue;
            }
            if (oneWord == "vn")
            {
                //            qDebug() << "Line is normal "  << QString::fromStdString(oneWord) << " ";
                QVector3D tempNormal;
                sStream >> oneWord;
                tempNormal.setX( std::stof(oneWord));
                sStream >> oneWord;
                tempNormal.setY(std::stof(oneWord));
                sStream >> oneWord;
                tempNormal.setZ(std::stof(oneWord));

                //Vertex made - pushing it into vertex-vector
                tempNormals.push_back(tempNormal);
                //tempColors.push_back(QVector3D{0,0,0}/*tempNormal*/);

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
                        tempMVertices.push_back(tempVert);
                        mesh->mVertices.push_back(tempVert);
                    }
                    else            //no uv in mesh data, use 0, 0 as uv
                    {
                        Vertex tempVert(tempVertecies[index], tempNormals[normal], QVector2D(0.0f, 0.0f));
                        tempMVertices.push_back(tempVert);
                        //mesh->mVertices.push_back(tempVert);
                    }
                    //mesh->mIndices.push_back(temp_index++);
                    tempMIndices.push_back(temp_index++);
                }
                continue;
            }
        }

        //beeing a nice boy and closing the file after use
        fileIn.close();
        //copy the object for future use

        obj.meshVert = tempMVertices;
        obj.meshIndic = tempMIndices;
        obj.collisionRadius = tempRadius;
        obj.internalIndex = meshDataContainer.size()-1;
        meshDataContainer.push_back(std::make_pair(input,obj));

        mesh->mVertices = tempMVertices;
        mesh->mIndices = tempMIndices;
        mesh->mVAO = meshDataContainer.size()-1;

        //mesh->mRVertices = &meshContainer.end()->second;
        //mesh->mRIndices = &meshIndiceContainer.end()->second;
    }//if the object already exists, we point the existing object to the new one
    else
    {
        //we find the correct mesh to copy here
        for(auto it = meshDataContainer.begin(); it < meshDataContainer.end();it++)
        {
            if(input == it->first){
                mesh->mVertices = it->second.meshVert;
                mesh->mIndices = it->second.meshIndic;
                mesh->collisionRadius = it->second.collisionRadius;
                mesh->mVAO = it->second.internalIndex;
                break;
            }
        }
    }



    qDebug() << "size of radius object: "  << mesh->collisionRadius << " \n";
    mesh->entity = 0;
    mesh->mDrawType = GL_TRIANGLES;

}


float resourceSystem::calculateLenght(QVector3D pos)
{   //we assume that the center of the obj is at 0,0,0
    //sqrt(x^2        +         y^2        +         z^2      ) = length
    return sqrt((pos.x()*pos.x()) + (pos.y()* pos.y()) + (pos.z()*pos.z()));
}


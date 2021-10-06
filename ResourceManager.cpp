#include "ResourceManager.h"

#include "renderwindow.h"
#include <sstream>

ResourceManager::ResourceManager()
{
    mCoreEngine = CoreEngine::getInstance();
}
GameObject* ResourceManager::CreateMainCharacter(std::string filename)
{
    //makeMainCharacter
    object = CreateObject(filename);
    object->transform->mMatrix.translate(57.f, -1.f, 6.f);

    return object;
}
GameObject* ResourceManager::CreateObject(std::string filename)
{
    object = new GameObject();
    object->transform = new Transform();
    object->transform->mMatrix.setToIdentity();
    object->material = new Material();
    object->mesh = new Mesh();

    object->mName = filename;

    int meshIndex{-1};
    auto result = mMeshIndexMap.find(filename);

    if(result != mMeshIndexMap.end()){
        meshIndex = result->second;

      //Gives the current objects mesh the same values as the "duplicate" version. (Updates mVAO, drawType, etc.)
        object->mesh = &mMeshComponents.at(meshIndex);
    }
    else{

        if(filename.find(".obj") != std::string::npos)
            meshIndex = readObj(gsl::MeshFilePath + filename);
        else
            qDebug() << "Entered else statement in ResourceManager::CreateObject"; //for testing

        mMeshIndexMap.emplace(filename, meshIndex);
        mMeshComponents[meshIndex] = *object->mesh;
    }

    if(meshIndex == -1){
        std::cout << "Error: meshIndex is -1 (no mesh)";
    }

    return object;
}


ResourceManager &ResourceManager::getInstance()
{
    static ResourceManager* mInstance = new ResourceManager();
    return *mInstance;
}

void ResourceManager::init(Mesh &meshComp, int lod)
{
    initializeOpenGLFunctions();

    glGenVertexArrays( 1, &meshComp.mVAO[lod] );
    glBindVertexArray( meshComp.mVAO[lod] );

    glGenBuffers( 1, &meshComp.mVBO[lod]);
    glBindBuffer( GL_ARRAY_BUFFER, meshComp.mVBO[lod] );

    glBufferData( GL_ARRAY_BUFFER, meshComp.mVertices[lod].size()*sizeof( Vertex ), meshComp.mVertices[lod].data(), GL_STATIC_DRAW );

    // 1st attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, meshComp.mVBO[lod]);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    if(meshComp.mIndices[lod].size() > 0) {
        glGenBuffers(1, &meshComp.mEAB[lod]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshComp.mEAB[lod]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshComp.mIndices[lod].size() * sizeof(GLuint), meshComp.mIndices[lod].data(), GL_STATIC_DRAW);
    }

    glBindVertexArray(0);
}

void ResourceManager::makeSphereRadius(Mesh *meshIn, gsl::Vector3D &vertexIn)
{
    //making correct bounding sphere radius:
    float length = vertexIn.length();
    if(length > meshIn->sphereRadius)
        meshIn->sphereRadius = length/1.75; //temporary... i get better results like this
}


int ResourceManager::readObj(std::string filename) //Ole's obj reader code
{
    //should check if this object is new before this!
        mMeshComponents.emplace_back(Mesh());
        *object->mesh = mMeshComponents.back();


        std::string tempString = filename;
        filename.erase(filename.find(".obj"));

        for (unsigned short lod{0}; lod < 3; lod++ )
        {
            if(lod != 0)
                tempString = filename + std::to_string(lod) + ".obj";

            std::ifstream fileIn;
            fileIn.open (tempString, std::ifstream::in);
            if(!fileIn)
                qDebug() << "Could not open file for reading: " << QString::fromStdString(tempString);

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

                    if(lod == 0)
                        makeSphereRadius(object->mesh, tempVertex);

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
                            object->mesh->mVertices[lod].push_back(tempVert);
                        }
                        else            //no uv in mesh data, use 0, 0 as uv
                        {
                            Vertex tempVert(tempVertecies[index], tempNormals[normal], gsl::Vector2D(0.0f, 0.0f));
                            object->mesh->mVertices[lod].push_back(tempVert);
                        }
                        object->mesh->mIndices[lod].push_back(temp_index++);
                    }
                    continue;
                }
            }
            fileIn.close();

            init(*object->mesh, lod);
        }

        return mMeshComponents.size()-1;    //returns index to last object
}

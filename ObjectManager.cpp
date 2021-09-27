#include "ObjectManager.h"

#include "renderwindow.h"

ObjectManager::ObjectManager()
{

}

GameObject* ObjectManager::CreateObject(std::string filename)
{

    int meshIndex{-1};


    object = new GameObject();

    object->mesh = new Mesh();


    auto result = mMeshIndexMap.find(filename);

    if(result != mMeshIndexMap.end()){
        meshIndex = result->second;
        qDebug() << "mesh is already made, use same meshIndex";
    }
    else{

        if(filename.find(".obj") != std::string::npos)
            meshIndex = readObj(gsl::MeshFilePath + filename);

        else if(filename.find("Cube") != std::string::npos)
            meshIndex = Cube(object->mesh); // quick test line, should not be hardcoded

        mMeshIndexMap.emplace(filename, meshIndex);
    }

    if(meshIndex == -1){
        std::cout << "meshIndex is -1 (no mesh)";
    }


    object->transform = new Transform();
    object->transform->mMatrix.setToIdentity();
    object->material = new Material();

//    mRenderwindow->addToGameObjects(object);

    return object;
}



void ObjectManager::init(Mesh &meshComp)
{
    initializeOpenGLFunctions();

    glGenVertexArrays( 1, &meshComp.mVAO );
    glBindVertexArray( meshComp.mVAO );

    glGenBuffers( 1, &meshComp.mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, meshComp.mVBO );

    glBufferData( GL_ARRAY_BUFFER, meshComp.mVertices.size()*sizeof( Vertex ), meshComp.mVertices.data(), GL_STATIC_DRAW );

    // 1st attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, meshComp.mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    if(meshComp.mIndices.size() > 0) {
        glGenBuffers(1, &meshComp.mEAB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshComp.mEAB);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshComp.mIndices.size() * sizeof(GLuint), meshComp.mIndices.data(), GL_STATIC_DRAW);
    }

        glBindVertexArray(0);
}


int ObjectManager::readObj(std::string filename) //Ole's obj reader code
{
    //should check if this object is new before this!
        mMeshComponents.emplace_back(Mesh());
        *object->mesh = mMeshComponents.back();

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
                        object->mesh->mVertices.push_back(tempVert);
                    }
                    else            //no uv in mesh data, use 0, 0 as uv
                    {
                        Vertex tempVert(tempVertecies[index], tempNormals[normal], gsl::Vector2D(0.0f, 0.0f));
                        object->mesh->mVertices.push_back(tempVert);
                    }
                    object->mesh->mIndices.push_back(temp_index++);
                }
                continue;
            }
        }

        //beeing a nice boy and closing the file after use
        fileIn.close();

        init(*object->mesh);

        return mMeshComponents.size()-1;    //returns index to last object
}


ObjectManager &ObjectManager::getInstance()
{
    static ObjectManager* mInstance = new ObjectManager();
    return *mInstance;
}

int ObjectManager::Cube(Mesh* meshComp)
{
    mMeshComponents.emplace_back(Mesh());
    Mesh &temp = mMeshComponents.back();

    // Front-1                           //Position           //RGB    //UV
    meshComp->mVertices.push_back(Vertex{-0.1, -0.1,  0.1,     1,0,0,   0,0});
    meshComp->mVertices.push_back(Vertex{ 0.1, -0.1,  0.1,     1,0,0,   1,0});
    meshComp->mVertices.push_back(Vertex{ 0.1,  0.1,  0.1,     1,0,0,   1,1});
    // Front-2                    1     1     1
    meshComp->mVertices.push_back(Vertex{ 0.1,  0.1,  0.1,     1,0,0,   0,0});
    meshComp->mVertices.push_back(Vertex{-0.1,  0.1,  0.1,     1,0,0,   1,0});
    meshComp->mVertices.push_back(Vertex{-0.1, -0.1,  0.1,     1,0,0,   1,1});
    // Back-1                     1     1     1
    meshComp->mVertices.push_back(Vertex{ 0.1, -0.1, -0.1,     1,0,0,   0,0});
    meshComp->mVertices.push_back(Vertex{-0.1, -0.1, -0.1,     1,0,0,   1,0});
    meshComp->mVertices.push_back(Vertex{-0.1,  0.1, -0.1,     1,0,0,   1,1});
    // Back-2                     1     1     1
    meshComp->mVertices.push_back(Vertex{-0.1,  0.1, -0.1,     1,0,0,   0,0});
    meshComp->mVertices.push_back(Vertex{ 0.1,  0.1, -0.1,     1,0,0,   1,0});
    meshComp->mVertices.push_back(Vertex{ 0.1, -0.1, -0.1,     1,0,0,   1,1});
    // Right-1                    1     1     1
    meshComp->mVertices.push_back(Vertex{ 0.1, -0.1,  0.1,     0,0,1,   0,0});
    meshComp->mVertices.push_back(Vertex{ 0.1, -0.1, -0.1,     0,0,1,   1,0});
    meshComp->mVertices.push_back(Vertex{ 0.1,  0.1, -0.1,     0,0,1,   1,1});
    // Right-2                    1     1     1
    meshComp->mVertices.push_back(Vertex{ 0.1,  0.1, -0.1,     0,0,1,   0,0});
    meshComp->mVertices.push_back(Vertex{ 0.1,  0.1,  0.1,     0,0,1,   1,0});
    meshComp->mVertices.push_back(Vertex{ 0.1, -0.1,  0.1,     0,0,1,   1,1});
    // Left-1                     1     1     1
    meshComp->mVertices.push_back(Vertex{-0.1, -0.1, -0.1,     0,0,1,   0,0});
    meshComp->mVertices.push_back(Vertex{-0.1, -0.1,  0.1,     0,0,1,   1,0});
    meshComp->mVertices.push_back(Vertex{-0.1,  0.1,  0.1,     0,0,1,   1,1});
    // Left-2                     1     1     1
    meshComp->mVertices.push_back(Vertex{-0.1,  0.1,  0.1,     0,0,1,   0,0});
    meshComp->mVertices.push_back(Vertex{-0.1,  0.1, -0.1,     0,0,1,   1,0});
    meshComp->mVertices.push_back(Vertex{-0.1, -0.1, -0.1,     0,0,1,   1,1});
    // Bot-1                      1     1     1
    meshComp->mVertices.push_back(Vertex{-0.1, -0.1,  0.1,     0,1,0,   0,0});
    meshComp->mVertices.push_back(Vertex{ 0.1, -0.1,  0.1,     0,1,0,   1,0});
    meshComp->mVertices.push_back(Vertex{ 0.1, -0.1, -0.1,     0,1,0,   1,1});
    // Bot-2                      1     1     1
    meshComp->mVertices.push_back(Vertex{ 0.1, -0.1, -0.1,     0,1,0,   0,0});
    meshComp->mVertices.push_back(Vertex{-0.1, -0.1, -0.1,     0,1,0,   1,0});
    meshComp->mVertices.push_back(Vertex{-0.1, -0.1,  0.1,     0,1,0,   1,1});
    // Top-1                      1     1     1
    meshComp->mVertices.push_back(Vertex{-0.1,  0.1,  0.1,     0,1,0,   0,0});
    meshComp->mVertices.push_back(Vertex{ 0.1,  0.1,  0.1,     0,1,0,   1,0});
    meshComp->mVertices.push_back(Vertex{ 0.1,  0.1, -0.1,     0,1,0,   1,1});
    // Top-2                      1     1     1
    meshComp->mVertices.push_back(Vertex{ 0.1,  0.1, -0.1,     0,1,0,   0,0});
    meshComp->mVertices.push_back(Vertex{-0.1,  0.1, -0.1,     0,1,0,   1,0});
    meshComp->mVertices.push_back(Vertex{-0.1,  0.1,  0.1,     0,1,0,   1,1});

    init(*object->mesh);

    return mMeshComponents.size()-1;
}

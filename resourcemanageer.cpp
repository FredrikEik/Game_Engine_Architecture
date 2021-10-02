#include "resourcemanageer.h"

ResourceManageer::ResourceManageer()
{

}

GameObject *ResourceManageer::addObject(std::string objName, std::vector<Vertex> vertexes)
{
    GameObject* tempObj = new GameObject();
    tempObj->mName = objName;
    tempObj->mTransform = new TransformComponent();
    tempObj->mTransform->mMatrix.setToIdentity();

    tempObj->mMesh = new MeshComponent();
    for(int i = 0; i < vertexes.size(); i++) tempObj->mMesh->mVertices.push_back(vertexes.at(i));

    tempObj->mMaterial = new MaterialComponent();
    tempObj->init();
    //tempObj->mTransform->mMatrix.translate(0.f, 0.f, .5f);
    return tempObj;
}

GameObject* readObj(std::string filename)
{
    //should check if this object is new before this!

    GameObject* tempObj = new GameObject();

    tempObj->mName = filename;
    tempObj->mTransform = new TransformComponent();
    tempObj->mTransform->mMatrix.setToIdentity();

    tempObj->mMesh = new MeshComponent();

    tempObj->mMaterial = new MaterialComponent();

    std::ifstream fileIn;
    fileIn.open (filename, std::ifstream::in);
    if(!fileIn)
    {
        qDebug() << "ERROR: Could not open file for reading: " << QString::fromStdString(filename);
        qDebug() << "****** using arbitrary mesh as replacement!";
        return 0;    //hack - this will crash if no meshes are made yet
    }
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
                    tempObj->mMesh->mVertices.push_back(tempVert);
                }
                else            //no uv in mesh data, use 0, 0 as uv
                {
                    Vertex tempVert(tempVertecies[index], tempNormals[normal], gsl::Vector2D(0.0f, 0.0f));
                    tempObj->mMesh->mVertices.push_back(tempVert);
                }
                tempObj->mMesh->mIndices.push_back(temp_index++);
            }
            continue;
        }
    }

    //beeing a nice boy and closing the file after use
    fileIn.close();

    qDebug() << QString::fromStdString(filename) << "successfully loaded";

    tempObj->init();

    return tempObj;    //returns index to last object
}

GameObject* ResourceManageer::addTriangle()
{
    GameObject* tempObj = new GameObject();
    tempObj->mName = "Triangle";
    tempObj->mTransform = new TransformComponent();
    tempObj->mTransform->mMatrix.setToIdentity();

    tempObj->mMesh = new MeshComponent();
    tempObj->mMesh->mVertices.push_back(Vertex{-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  0.f, 0.f}); // Bottom Left
    tempObj->mMesh->mVertices.push_back(Vertex{0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.f}); // Bottom Right
    tempObj->mMesh->mVertices.push_back(Vertex{0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.f}); // Top

    tempObj->mMaterial = new MaterialComponent();
    tempObj->mMesh->mDrawType = GL_TRIANGLES;
    tempObj->init();
    tempObj->mTransform->mMatrix.translate(0.f, 0.f, .5f);
    return tempObj;
}

GameObject* ResourceManageer::addCube()
{
    GameObject* tempObj = new GameObject();
    tempObj->mName = "Cube";
    tempObj->mTransform = new TransformComponent();
    tempObj->mTransform->mMatrix.setToIdentity();

    tempObj->mMesh = new MeshComponent();
    // Front-1
        tempObj->mMesh->mVertices.push_back(Vertex{ -0.5, -0.5,  0.5, 1,0,0});
        tempObj->mMesh->mVertices.push_back(Vertex{  0.5, -0.5,  0.5, 1,0,0});
        tempObj->mMesh->mVertices.push_back(Vertex{  0.5,  0.5,  0.5, 1,0,0});
        // Front-2
        tempObj->mMesh->mVertices.push_back(Vertex{  0.5,  0.5,  0.5, 1,1,0});
        tempObj->mMesh->mVertices.push_back(Vertex{ -0.5,  0.5,  0.5, 1,1,0});
        tempObj->mMesh->mVertices.push_back(Vertex{ -0.5, -0.5,  0.5, 1,1,0});

        // Back-1
        tempObj->mMesh->mVertices.push_back(Vertex{  0.5, -0.5, -0.5, 1,1,1});
        tempObj->mMesh->mVertices.push_back(Vertex{ -0.5, -0.5, -0.5, 1,1,1});
        tempObj->mMesh->mVertices.push_back(Vertex{ -0.5,  0.5, -0.5, 1,1,1});
        // Back-2
        tempObj->mMesh->mVertices.push_back(Vertex{ -0.5,  0.5, -0.5, 0,1,1});
        tempObj->mMesh->mVertices.push_back(Vertex{  0.5,  0.5, -0.5, 0,1,1});
        tempObj->mMesh->mVertices.push_back(Vertex{  0.5, -0.5, -0.5, 0,1,1});

        // Right-1
        tempObj->mMesh->mVertices.push_back(Vertex{  0.5, -0.5,  0.5, 1,0,1});
        tempObj->mMesh->mVertices.push_back(Vertex{  0.5, -0.5, -0.5, 1,0,1});
        tempObj->mMesh->mVertices.push_back(Vertex{  0.5,  0.5, -0.5, 1,0,1});
        // Right-2
        tempObj->mMesh->mVertices.push_back(Vertex{  0.5,  0.5, -0.5, 1,0.5,0});
        tempObj->mMesh->mVertices.push_back(Vertex{  0.5,  0.5,  0.5, 1,0.5,0});
        tempObj->mMesh->mVertices.push_back(Vertex{  0.5, -0.5,  0.5, 1,0.5,0});

        // Left-1
        tempObj->mMesh->mVertices.push_back(Vertex{ -0.5, -0.5, -0.5, 1,0.5,1});
        tempObj->mMesh->mVertices.push_back(Vertex{ -0.5, -0.5,  0.5, 1,0.5,1});
        tempObj->mMesh->mVertices.push_back(Vertex{ -0.5,  0.5,  0.5, 1,0.5,1});
        // Left-2
        tempObj->mMesh->mVertices.push_back(Vertex{ -0.5,  0.5,  0.5, 0.5,0.5,0});
        tempObj->mMesh->mVertices.push_back(Vertex{ -0.5,  0.5, -0.5, 0.5,0.5,0});
        tempObj->mMesh->mVertices.push_back(Vertex{ -0.5, -0.5, -0.5, 0.5,0.5,0});

        // Bot-1
        tempObj->mMesh->mVertices.push_back(Vertex{ -0.5, -0.5,  0.5, 0.5,0.5,0.5});
        tempObj->mMesh->mVertices.push_back(Vertex{  0.5, -0.5,  0.5, 0.5,0.5,0.5});
        tempObj->mMesh->mVertices.push_back(Vertex{  0.5, -0.5, -0.5, 0.5,0.5,0.5});
        // Bot-2
        tempObj->mMesh->mVertices.push_back(Vertex{  0.5, -0.5, -0.5, 0.5,0.5,1});
        tempObj->mMesh->mVertices.push_back(Vertex{ -0.5, -0.5, -0.5, 0.5,0.5,1});
        tempObj->mMesh->mVertices.push_back(Vertex{ -0.5, -0.5,  0.5, 0.5,0.5,1});

        // Top-1
        tempObj->mMesh->mVertices.push_back(Vertex{ -0.5,  0.5,  0.5, 0.5,0,0});
        tempObj->mMesh->mVertices.push_back(Vertex{  0.5,  0.5,  0.5, 0.5,0,0});
        tempObj->mMesh->mVertices.push_back(Vertex{  0.5,  0.5, -0.5, 0.5,0,0});
        // Top-2
        tempObj->mMesh->mVertices.push_back(Vertex{  0.5,  0.5, -0.5, 0,0.5,0});
        tempObj->mMesh->mVertices.push_back(Vertex{ -0.5,  0.5, -0.5, 0,0.5,0});
        tempObj->mMesh->mVertices.push_back(Vertex{ -0.5,  0.5,  0.5, 0,0.5,0});


    tempObj->mMaterial = new MaterialComponent();
    tempObj->mMesh->mDrawType = GL_TRIANGLES;
    tempObj->init();
    tempObj->mTransform->mMatrix.translate(0.f, 0.f, .5f);
    return tempObj;
}

GameObject *ResourceManageer::addXYZ()
{
    GameObject* tempObj = new GameObject();
    tempObj->mName = "XYZ";
    tempObj->mTransform = new TransformComponent();
    tempObj->mTransform->mMatrix.setToIdentity();

    tempObj->mMesh = new MeshComponent();

    tempObj->mMesh->mVertices.push_back(Vertex{0.f, 0.f, 0.f, 1.f, 0.f, 0.f});
    tempObj->mMesh->mVertices.push_back(Vertex{100.f, 0.f, 0.f, 1.f, 0.f, 0.f});
    tempObj->mMesh->mVertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 1.f, 0.f});
    tempObj->mMesh->mVertices.push_back(Vertex{0.f, 100.f, 0.f, 0.f, 1.f, 0.f});
    tempObj->mMesh->mVertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 0.f, 1.f});
    tempObj->mMesh->mVertices.push_back(Vertex{0.f, 0.f, 100.f, 0.f, 0.f, 1.f});

    tempObj->mMesh->mDrawType = GL_LINES;

    tempObj->mMaterial = new MaterialComponent();
    tempObj->init();

    return tempObj;
}

GameObject *ResourceManageer::objectCreator(std::string objName)
{
    int index;
    auto temp = meshComponentMap.find(objName);
    if(temp == meshComponentMap.end())
    {
        if(objName == "Triangle" || "triangle")
        {
            index = standardTriangle();
        }
        else if(objName == "Cube" || "cube")
        {
            index = standardCube();
        }
    }
    else{
        if(temp->first == "Triangle" || "triangle")
        {
            index = temp->second;
        }
        else if(temp->first == "Cube" || "cube")
        {
            index = temp->second;
        }
    }
    GameObject* tempObj = new GameObject();
    tempObj->mName = objName;
    tempObj->mTransform = new TransformComponent();
    tempObj->mTransform->mMatrix.setToIdentity();
    tempObj->mMaterial = new MaterialComponent();
    tempObj->mMesh = meshComponents.at(index);
    tempObj->init();

    return tempObj;
}

int ResourceManageer::standardTriangle()
{
    MeshComponent* temp = new MeshComponent;
    temp->mVertices.push_back(Vertex{-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  0.f, 0.f});
    temp->mVertices.push_back(Vertex{0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.f});
    temp->mVertices.push_back(Vertex{0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.f});
    temp->mDrawType = GL_TRIANGLES;
    meshComponents.emplace_back(temp);
    meshComponentMap.emplace("Triangle", meshComponentMap.size()-1);
    return meshComponents.size()-1;
}

int ResourceManageer::standardCube()
{
    meshComponentMap.emplace("Triangle");
    MeshComponent* temp = new MeshComponent;
    temp->mVertices.push_back(Vertex{ -0.5, -0.5,  0.5, 1,0,0});
    temp->mVertices.push_back(Vertex{  0.5, -0.5,  0.5, 1,0,0});
    temp->mVertices.push_back(Vertex{  0.5,  0.5,  0.5, 1,0,0});
    temp->mVertices.push_back(Vertex{  0.5,  0.5,  0.5, 1,1,0});
    temp->mVertices.push_back(Vertex{ -0.5,  0.5,  0.5, 1,1,0});
    temp->mVertices.push_back(Vertex{ -0.5, -0.5,  0.5, 1,1,0});
    temp->mVertices.push_back(Vertex{  0.5, -0.5, -0.5, 1,1,1});
    temp->mVertices.push_back(Vertex{ -0.5, -0.5, -0.5, 1,1,1});
    temp->mVertices.push_back(Vertex{ -0.5,  0.5, -0.5, 1,1,1});
    temp->mVertices.push_back(Vertex{ -0.5,  0.5, -0.5, 0,1,1});
    temp->mVertices.push_back(Vertex{  0.5,  0.5, -0.5, 0,1,1});
    temp->mVertices.push_back(Vertex{  0.5, -0.5, -0.5, 0,1,1});
    temp->mVertices.push_back(Vertex{  0.5, -0.5,  0.5, 1,0,1});
    temp->mVertices.push_back(Vertex{  0.5, -0.5, -0.5, 1,0,1});
    temp->mVertices.push_back(Vertex{  0.5,  0.5, -0.5, 1,0,1});
    temp->mVertices.push_back(Vertex{  0.5,  0.5, -0.5, 1,0.5,0});
    temp->mVertices.push_back(Vertex{  0.5,  0.5,  0.5, 1,0.5,0});
    temp->mVertices.push_back(Vertex{  0.5, -0.5,  0.5, 1,0.5,0});
    temp->mVertices.push_back(Vertex{ -0.5, -0.5, -0.5, 1,0.5,1});
    temp->mVertices.push_back(Vertex{ -0.5, -0.5,  0.5, 1,0.5,1});
    temp->mVertices.push_back(Vertex{ -0.5,  0.5,  0.5, 1,0.5,1});
    temp->mVertices.push_back(Vertex{ -0.5,  0.5,  0.5, 0.5,0.5,0});
    temp->mVertices.push_back(Vertex{ -0.5,  0.5, -0.5, 0.5,0.5,0});
    temp->mVertices.push_back(Vertex{ -0.5, -0.5, -0.5, 0.5,0.5,0});
    temp->mVertices.push_back(Vertex{ -0.5, -0.5,  0.5, 0.5,0.5,0.5});
    temp->mVertices.push_back(Vertex{  0.5, -0.5,  0.5, 0.5,0.5,0.5});
    temp->mVertices.push_back(Vertex{  0.5, -0.5, -0.5, 0.5,0.5,0.5});
    temp->mVertices.push_back(Vertex{  0.5, -0.5, -0.5, 0.5,0.5,1});
    temp->mVertices.push_back(Vertex{ -0.5, -0.5, -0.5, 0.5,0.5,1});
    temp->mVertices.push_back(Vertex{ -0.5, -0.5,  0.5, 0.5,0.5,1});
    temp->mVertices.push_back(Vertex{ -0.5,  0.5,  0.5, 0.5,0,0});
    temp->mVertices.push_back(Vertex{  0.5,  0.5,  0.5, 0.5,0,0});
    temp->mVertices.push_back(Vertex{  0.5,  0.5, -0.5, 0.5,0,0});
    temp->mVertices.push_back(Vertex{  0.5,  0.5, -0.5, 0,0.5,0});
    temp->mVertices.push_back(Vertex{ -0.5,  0.5, -0.5, 0,0.5,0});
    temp->mVertices.push_back(Vertex{ -0.5,  0.5,  0.5, 0,0.5,0});
    temp->mDrawType = GL_TRIANGLES;
    meshComponents.emplace_back(temp);
    return meshComponents.size()-1;
}


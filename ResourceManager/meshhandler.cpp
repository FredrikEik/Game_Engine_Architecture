﻿#include "ResourceManager/meshhandler.h"

#include <sstream>
#include <fstream>

#include "constants.h"
#include "math_constants.h"
#include "camera.h"
#include "logger.h"
//#include "gameobjectmanager.h"
//#include "gameobject.h"

MeshHandler::MeshHandler()
{
    //This is a hack - to make sure my MeshComponens are not moved for now
    //Please fix
    mMeshes.reserve(gsl::MAX_MESHCOMPONENTS);
    mLogger = Logger::getInstance();    //Have to do this, else program will crash
//    mGameObjectManager = &GameObjectManager::getInstance();
}

int MeshHandler::makeMesh(std::string meshName)
{
    int meshIndex{-1};

    //check if asset is made:
    auto result = mMeshMap.find(meshName);
    //if already made
    if (result != mMeshMap.end()) {        //found!!!
//        qDebug() << meshName.c_str() << "made already";
        meshIndex = result->second;
    }
    //not made, make new
    else {

        //Simple "factory" - making the meshobject said in the filename
        if (meshName.find(".obj") != std::string::npos)
            meshIndex = readObj(meshName);
        if (meshName.find("axis") != std::string::npos)
            meshIndex = makeAxis();
        if (meshName.find("triangle") != std::string::npos)
            meshIndex = makeTriangle();
        if (meshName.find("editorgrid") != std::string::npos)
            meshIndex = makeEditorGrid();

        //If nothing matches meshName - just make a triangle
        //Fix - this will make duplicate triangles
        if(meshIndex == -1)
            meshIndex = makeTriangle();

        //update mMeshComponentMap with new asset
        mMeshMap.emplace(meshName, meshIndex);
    }

    return meshIndex;
}

int MeshHandler::readObj(std::string filename)
{
    mMeshes.emplace_back(MeshData());
    MeshData &temp = mMeshes.back();

    std::string tempName{0};
    filename.erase(filename.find(".obj"));  //deleting .obj ending to make LOD-level filenames

    for (unsigned short lod{0}; lod < 3; lod++ )  //we test for 3 LOD levels
    {
        if (lod == 0)     //original mesh - not reduced size
            tempName = filename;
        else
            tempName = filename + gsl::LODLevelPrefix + std::to_string(lod);

        tempName = gsl::MeshFilePath + tempName + ".obj";
        qDebug() << "Reading " << tempName.c_str();
        std::ifstream fileIn;
        fileIn.open (tempName, std::ifstream::in);
        if(!fileIn)
        {
            mLogger->logText("ERROR: Could not open file for reading: " + filename, LColor::DAMNERROR);
            mLogger->logText("****** using arbitrary mesh as replacement!", LColor::DAMNERROR);
            return makeTriangle();    //returns triangle instead
        }
        //One line at a time-variable
        std::string oneLine;
        //One word at a time-variable
        std::string oneWord;

        std::vector<gsl::Vector3D> tempVertecies;
        std::vector<gsl::Vector3D> tempNormals;
        std::vector<gsl::Vector2D> tempUVs;

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

                //make collider only for LOD 0 == original mesh
                if(lod == 0)
                    makeColliderCorners(temp, tempVertex);

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
                        temp.mVertices[lod].push_back(tempVert);
                    }
                    else            //no uv in mesh data, use 0, 0 as uv
                    {
                        Vertex tempVert(tempVertecies[index], tempNormals[normal], gsl::Vector2D(0.0f, 0.0f));
                        temp.mVertices[lod].push_back(tempVert);
                    }
                    temp.mIndices[lod].push_back(temp_index++);
                }
                continue;
            }
        }
        //beeing a nice boy and closing the file after use
        fileIn.close();

        initMesh(temp, lod);
    }

    mLogger->logText(filename + " successfully loaded");

    return mMeshes.size()-1;    //returns index to last object
}

int MeshHandler::makeAxis()
{
    //should check if this object is new before this!
    mMeshes.emplace_back(MeshData());
    MeshData &temp = mMeshes.back();

    temp.mVertices[0].push_back(Vertex{0.f, 0.f, 0.f, 1.f, 0.f, 0.f});
    temp.mVertices[0].push_back(Vertex{100.f, 0.f, 0.f, 1.f, 0.f, 0.f});
    temp.mVertices[0].push_back(Vertex{0.f, 0.f, 0.f, 0.f, 1.f, 0.f});
    temp.mVertices[0].push_back(Vertex{0.f, 100.f, 0.f, 0.f, 1.f, 0.f});
    temp.mVertices[0].push_back(Vertex{0.f, 0.f, 0.f, 0.f, 0.f, 1.f});
    temp.mVertices[0].push_back(Vertex{0.f, 0.f, 100.f, 0.f, 0.f, 1.f});

    temp.mDrawType = GL_LINES;
    temp.mColliderRadius = 1.f; //Axis does not have a collider...

    //only LOD level 0
    initMesh(temp, 0);

    return mMeshes.size()-1;    //returns index to last object
}

int MeshHandler::makeTriangle()
{
    //should check if this object is new before this!
    mMeshes.emplace_back(MeshData());
    MeshData &temp = mMeshes.back();
    temp.mVertices[0].push_back(Vertex{-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  0.f, 0.f}); // Bottom Left
    temp.mVertices[0].push_back(Vertex{0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.f}); // Bottom Right
    temp.mVertices[0].push_back(Vertex{0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.f}); // Top

    temp.mDrawType = GL_TRIANGLES;

    //The 0, 0, 0 point of this triangle is not in center of visual triangle
    //so this is a little bigger than it needs to
    temp.mColliderRadius = 0.7071068f;  //sqrt(0.5);

    //only LOD level 0
    initMesh(temp, 0);

    return mMeshes.size()-1;    //returns index to last object
}

int MeshHandler::makeEditorGrid(int size, int scale)
{
    //should check if this object is new before this!
    mMeshes.emplace_back(MeshData());
    MeshData &temp = mMeshes.back();

    float startPoint = -size * scale;

    float lineStart = startPoint;
    float lineEnd = -startPoint;

    float mainColor{0.3f};
    float altColor{0.5f};
    float color{0.f};

    bool altColorFlip{true};    //alternates between two colors

    float altMainAxisColor{.45f};
    float yHeight{-0.001f};     //to avoid z-fighting with axis

    //lines parallell to Z
    for(float i{startPoint}; i<= (startPoint*-1); i += scale)
    {
        //special case for 0 line:
        if (i >=-0.1f && i<= 0.1f)
        {
            temp.mVertices[0].push_back(Vertex{i, yHeight, lineStart,       altMainAxisColor, altMainAxisColor, 1.f});
            temp.mVertices[0].push_back(Vertex{i, yHeight, lineEnd,         altMainAxisColor, altMainAxisColor, 1.f});
            altColorFlip = !altColorFlip;
        }
        else
        {
            altColorFlip ? color = mainColor : color = altColor;
            temp.mVertices[0].push_back(Vertex{i, yHeight, lineStart, color, color, color});
            temp.mVertices[0].push_back(Vertex{i, yHeight, lineEnd, color, color, color});
            altColorFlip = !altColorFlip;
        }
    }

    altColorFlip = true;    //reset for X-lines
    //lines parallell to X
    for(float i{startPoint}; i<= (startPoint*-1); i += scale)
    {
        //special case for  0 line:
        if (i >=-0.1f && i<= 0.1f)
        {
            temp.mVertices[0].push_back(Vertex{lineStart, yHeight, i,       1.f, altMainAxisColor, altMainAxisColor});
            temp.mVertices[0].push_back(Vertex{lineEnd, yHeight, i,         1.f, altMainAxisColor, altMainAxisColor});
            altColorFlip = !altColorFlip;
        }
        else
        {
            altColorFlip ? color = mainColor : color = altColor;
            temp.mVertices[0].push_back(Vertex{lineStart, yHeight, i, color, color, color});
            temp.mVertices[0].push_back(Vertex{lineEnd, yHeight, i, color, color, color});
            altColorFlip = !altColorFlip;
        }
    }

    temp.mColliderRadius = 1.f; //grid should have no collider...
    temp.mDrawType = GL_LINES;
    //only LOD level 0
    initMesh(temp, 0);
    return mMeshes.size()-1;
}

MeshData MeshHandler::makeLine(gsl::Vector3D &positionIn, int direction, float lenght, gsl::Vector3D colorIn)
{
    MeshData tempMesh;
    tempMesh.mVertices[0].push_back(Vertex{0.f, 0.f, 0.f,   colorIn.x, colorIn.y, colorIn.z,  0.f, 0.f});

    if(direction == 1)  //X axis
        tempMesh.mVertices[0].push_back(Vertex{1.f * lenght, 0.f, 0.f,   colorIn.x, colorIn.y, colorIn.z,  0.f, 0.f});
    if(direction == 2)  //Y axis
        tempMesh.mVertices[0].push_back(Vertex{0.f, 1.f * lenght, 0.f,   colorIn.x, colorIn.y, colorIn.z,  0.f, 0.f});
    if(direction == 3)  //Z axis
        tempMesh.mVertices[0].push_back(Vertex{0.f, 0.f, 1.f * lenght,   colorIn.x, colorIn.y, colorIn.z,  0.f, 0.f});

    tempMesh.mDrawType = GL_LINES;

    //only LOD level 0
    initMesh(tempMesh, 0);

    return tempMesh;
}

MeshData MeshHandler::makeLine(gsl::Vector3D &startIn, gsl::Vector3D endIn, float lenght, gsl::Vector3D colorIn)
{
    MeshData tempMesh;
    endIn = endIn * lenght;
    tempMesh.mVertices[0].push_back(Vertex{startIn.x, startIn.y, startIn.z,   colorIn.x, colorIn.y, colorIn.z,  0.f, 0.f});
    tempMesh.mVertices[0].push_back(Vertex{endIn.x, endIn.y, endIn.z,   colorIn.x, colorIn.y, colorIn.z,  0.f, 0.f});

    tempMesh.mDrawType = GL_LINES;

    //only LOD level 0
    initMesh(tempMesh, 0);

    return tempMesh;
}

MeshData MeshHandler::makePoint(gsl::Vector3D &pointIn, float pointSizeIn, gsl::Vector3D colorIn)
{
    MeshData tempMesh;

    tempMesh.mVertices->push_back(Vertex{pointIn.x, pointIn.y, pointIn.z, colorIn.x, colorIn.y, colorIn.z, 0.0f, 0.0f});

    tempMesh.mDrawType = GL_POINTS;

    initMesh(tempMesh, 0);

    return tempMesh;
}

//void MeshHandler::makeMeshFromMeshData(MeshData tempMesh)
//{
//    mMeshComponents.emplace_back(tempMesh);
//    *object->mMesh = mMeshComponents.back();

//    object->mMesh->mVertices->push_back(Vertex(vert[i].getXYZ().x -xPos, lineHeight, vert[i].getXYZ().z, 1,0,0));
//}

MeshData MeshHandler::makeCircleSphere(float radius, bool rgbColor)
{
    MeshData tempMesh;

    float angle = gsl::deg2radf(90/6);

    //X-axis circle
    float zStart = radius;
    float yStart = 0.f;

    //pink
    gsl::Vector3D color{1.f, 0.301f, 0.933f};

    if (rgbColor)
        color = gsl::Vector3D(1.f, 0.f, 0.f);

    for (int i{0}; i < 24 ; i++)
    {
        float tempAngle = angle*i;
        float zNew = zStart * cosf(tempAngle) - yStart * sinf(tempAngle);
        float yNew = zStart * sinf(tempAngle) + yStart * cosf(tempAngle);

        tempMesh.mVertices[0].push_back(Vertex{0.f, yNew, zNew,    color.x, color.y, color.z,   0.f, 0.f});
    }

    //Y-axis circle
    zStart = radius;
    float xStart = 0.f;
    if (rgbColor)
        color = gsl::Vector3D(0.f, 1.f, 0.f);

    for (int i{0}; i < 24 ; i++)
    {
        float tempAngle = angle*i;
        float zNew = zStart * cosf(tempAngle) - xStart * sinf(tempAngle);
        float xNew = zStart * sinf(tempAngle) + xStart * cosf(tempAngle);

        tempMesh.mVertices[0].push_back(Vertex{xNew, 0.f, zNew,      color.x, color.y, color.z,   0.f, 0.f});
    }

    //Z-axis circle
    xStart = radius;
    yStart = 0.f;
    if (rgbColor)
        color = gsl::Vector3D(0.f, 0.f, 1.f);

    for (int i{0}; i < 24 ; i++)
    {
        float tempAngle = angle*i;
        float xNew = xStart * cosf(tempAngle) - yStart * sinf(tempAngle);
        float yNew = xStart * sinf(tempAngle) + yStart * cosf(tempAngle);

        tempMesh.mVertices[0].push_back(Vertex{xNew, yNew, 0.f,      color.x, color.y, color.z,   0.f, 0.f});
    }

    //Making indices:
    for (int i{0}; i < 23 ; i++)
    {
        tempMesh.mIndices[0].push_back(i);
        tempMesh.mIndices[0].push_back(i+1);
    }
    tempMesh.mIndices[0].push_back(23);
    tempMesh.mIndices[0].push_back(0);

    for (int i{24}; i < 47 ; i++)
    {
        tempMesh.mIndices[0].push_back(i);
        tempMesh.mIndices[0].push_back(i+1);
    }
    tempMesh.mIndices[0].push_back(47);
    tempMesh.mIndices[0].push_back(24);

    for (int i{48}; i < 71 ; i++)
    {
        tempMesh.mIndices[0].push_back(i);
        tempMesh.mIndices[0].push_back(i+1);
    }
    tempMesh.mIndices[0].push_back(71);
    tempMesh.mIndices[0].push_back(48);

    tempMesh.mDrawType = GL_LINES;

    //only LOD level 0
    initMesh(tempMesh, 0);

    return tempMesh;
}

MeshData MeshHandler::makeFrustum(const Frustum &frustumIn)
{
    //calculate corners of frustum:
    //Math shown here: https://learnopengl.com/Guest-Articles/2021/Scene/Frustum-Culling
    float tanFOVv = tanf(gsl::deg2radf(frustumIn.mFOVvertical/2));          // expensive calculation - save answer

    float halfHeightFar = abs(frustumIn.mFarPlaneDistance * tanFOVv);
    float halfWidthFar = halfHeightFar * frustumIn.mAspectRatio;

    float halfHeightNear = abs(frustumIn.mNearPlaneDistance * tanFOVv);
    float halfWidthNear = halfHeightNear * frustumIn.mAspectRatio;

    // camera looks down -Z (as a start) so near and far-plane are negative when drawn
    gsl::Vector3D cornerNear = gsl::Vector3D(halfWidthNear, halfHeightNear, -frustumIn.mNearPlaneDistance);
    gsl::Vector3D cornerFar = gsl::Vector3D(halfWidthFar, halfHeightFar, -frustumIn.mFarPlaneDistance);

    MeshData tempMesh;

    tempMesh.mVertices[0].insert( tempMesh.mVertices[0].end(),
      {         //Vertex data for front points                  color                       uv
       Vertex{-cornerNear.x, -cornerNear.y, cornerNear.z,       1.f, 0.301f, 0.933f,          0.f, 0.f},     // 0
       Vertex{cornerNear.x,  -cornerNear.y, cornerNear.z,       1.f, 0.301f, 0.933f,          0.f, 0.f},
       Vertex{cornerNear.x,  cornerNear.y,  cornerNear.z,       1.f, 0.301f, 0.933f,          0.f, 0.f},
       Vertex{-cornerNear.x, cornerNear.y,  cornerNear.z,       1.f, 0.301f, 0.933f,          0.f, 0.f},
       //Vertex data for back
       Vertex{-cornerFar.x, -cornerFar.y, cornerFar.z,          1.f, 0.301f, 0.933f,          0.f, 0.f},    // 4
       Vertex{cornerFar.x,  -cornerFar.y, cornerFar.z,          1.f, 0.301f, 0.933f,          0.f, 0.f},
       Vertex{cornerFar.x,  cornerFar.y,  cornerFar.z,          1.f, 0.301f, 0.933f,          0.f, 0.f},
       Vertex{-cornerFar.x, cornerFar.y,  cornerFar.z,          1.f, 0.301f, 0.933f,          0.f, 0.f},
                      });

    //One line at a time
    tempMesh.mIndices[0].insert( tempMesh.mIndices[0].end(),
    { 0, 1, 1, 2, 2, 3, 3, 0,       //front rectangle
      4, 5, 5, 6, 6, 7, 7, 4,       //back rectangle
      0, 4, 3, 7,                   //leftside lines
      1, 5, 2, 6                    //rightside lines
                     });

    tempMesh.mDrawType = GL_LINES;

    //only LOD level 0
    initMesh(tempMesh, 0);

    return tempMesh;
}

MeshData MeshHandler::makeLineBox(std::string meshName)
{
    auto result = mMeshMap.find(meshName);
    gsl::Vector3D &lowLeftBack = mMeshes[result->second].mLowLeftBackCorner;
    gsl::Vector3D &upRightFront = mMeshes[result->second].mUpRightFrontCorner;
    MeshData temp;
    float xMinus{lowLeftBack.x};
    float xPlus{upRightFront.x};
    float yMinus{lowLeftBack.y};
    float yPlus{upRightFront.y};
    float zMinus{lowLeftBack.z};
    float zPlus{upRightFront.z};

    temp.mVertices[0].insert( temp.mVertices[0].end(),
      {//Vertex data for front points       color                       uv
       Vertex{xMinus, yMinus, zPlus,       1, 0.301, 0.933,          0.f, 0.f},     // 0
       Vertex{xPlus,  yMinus, zPlus,       1, 0.301, 0.933,          0.f, 0.f},
       Vertex{xPlus,  yPlus,  zPlus,       1, 0.301, 0.933,          0.f, 0.f},
       Vertex{xMinus, yPlus,  zPlus,       1, 0.301, 0.933,          0.f, 0.f},
       //Vertex data for back
       Vertex{xMinus, yMinus, zMinus,      1, 0.301, 0.933,          0.f, 0.f},    // 4
       Vertex{xPlus,  yMinus, zMinus,      1, 0.301, 0.933,          0.f, 0.f},
       Vertex{xPlus,  yPlus,  zMinus,      1, 0.301, 0.933,          0.f, 0.f},
       Vertex{xMinus, yPlus,  zMinus,      1, 0.301, 0.933,          0.f, 0.f},
                      });

    //One line at a time
    temp.mIndices[0].insert( temp.mIndices[0].end(),
    { 0, 1, 1, 2, 2, 3, 3, 0,       //front rectangle
      4, 5, 5, 6, 6, 7, 7, 4,       //back rectangle
      0, 4, 3, 7,                   //leftside lines
      1, 5, 2, 6                    //rightside lines
                     });

    temp.mDrawType = GL_LINES;

    //only LOD level 0
    initMesh(temp, 0);

    return temp;
}

void MeshHandler::makeColliderCorners(MeshData &meshIn, gsl::Vector3D &vertexIn)
{
    //testing min
    if(vertexIn.x < meshIn.mLowLeftBackCorner.x)
        meshIn.mLowLeftBackCorner.x = vertexIn.x;
    if(vertexIn.y < meshIn.mLowLeftBackCorner.y)
        meshIn.mLowLeftBackCorner.y = vertexIn.y;
    if(vertexIn.z < meshIn.mLowLeftBackCorner.z)
        meshIn.mLowLeftBackCorner.z = vertexIn.z;

    //testing max
    if(vertexIn.x > meshIn.mUpRightFrontCorner.x)
        meshIn.mUpRightFrontCorner.x = vertexIn.x;
    if(vertexIn.y > meshIn.mUpRightFrontCorner.y)
        meshIn.mUpRightFrontCorner.y = vertexIn.y;
    if(vertexIn.z > meshIn.mUpRightFrontCorner.z)
        meshIn.mUpRightFrontCorner.z = vertexIn.z;

    //making correct bounding sphere radius:
    float length = vertexIn.length();
    if(length > meshIn.mColliderRadius)
        meshIn.mColliderRadius = length;
}

void MeshHandler::readLasFile()
{
    float xMin{0}, xMax{0}, yMin{0}, yMax{0}, zMin{0}, zMax{0};

    std::ifstream inLasFile;
    inLasFile.open(gsl::ProjectFolderName + "Assets/test_las.txt");

    if(!inLasFile)
    {
        qDebug() << "unable to open file test_las.txt";
        return;
    }

    std::vector<gsl::Vector3D> pointData;
    int linesWithPoints = {0};
    float lasX, lasY, lasZ = {0};

    if(inLasFile.is_open())
    {
         qDebug() << "Opening las file";
         inLasFile >> linesWithPoints; //read first line, the approximate number of datalines.
         pointData.reserve(linesWithPoints); //reserve the space for the number of datalines.

         for(int i = 0; i < linesWithPoints; i++) //for each line of data fill pointData with numbers
         {
             inLasFile >> lasX >> lasZ >> lasY; //In the Las file the data is stored x,z,y
             pointData.emplace_back(lasX, lasY, lasZ); //I fill the pointdata with x,y,z.
             //// NOTE - There is some "noise" after 2 decimals when filling into the vector3d, but not significantly so.
             //// Given all data will use the same pointData vector this wont be a problem.

             static bool once = true; //store the bounds of the terrain
             if(once)
             {
                 xMin = lasX;
                 xMax = lasX;
                 zMin = lasZ;
                 zMax = lasZ;
                 yMin = lasY;
                 yMax = lasY;
                 once = false;
             }
             if(lasX < xMin)
                 xMin = lasX;
             if(lasX > xMax)
                 xMax = lasX;
             if(lasZ < zMin)
                 zMin = lasZ;
             if(lasZ > zMax)
                 zMax = lasZ;
             if(lasY < yMin)
                 yMin = lasY;
             if(lasY > yMax)
                 yMax = lasY;

         }
    }
    inLasFile.close();
    qDebug() << "Closing las file";
//    qDebug() << xMin << xMax << yMin << yMax << zMin << zMax;
//    qDebug() << "inLasFile lines with points" << linesWithPoints << "PointData Size" << pointData.size();
//    qDebug() << "x" << pointData[0].x << "y" << pointData[0].y << "z" << pointData[0].z;


//--------------Done reading las file, on to creating a simplification-------------------//


// I have made an assumption that the grid is square, will have another look at this if something goes wrong
// could use xMax and zMax to get correct rectangle
    const int gridSizeX = 5; // This serves as a "resolution" now the grid with a huge set of datapoints, only
    const int gridSizeZ = 5; // have 50x50 = 2500 points for further work.

//Keeping this in case i need further accuracy, including all points
    {
//Fill outer bounds of a 2D grid.
//    planeGrid[0][0].x = xMin; //Low left
//    planeGrid[0][0].y = zMin; //Low left

//    planeGrid[gridSizeX-1][gridSizeZ-1].x = xMax; //top right
//    planeGrid[gridSizeX-1][gridSizeZ-1].y = zMax; //top right

//    planeGrid[gridSizeX-1][0].x = xMax; //bot right
//    planeGrid[0][gridSizeX-1].y = zMin; //top left

//    planeGrid[0][gridSizeZ-1].x = xMin; //top left
//    planeGrid[gridSizeZ-1][0].y = zMax; //bot right
    }

    //Fill the rest of the grid with evenly spaced coordinates between min and max.
    float distanceBetweenSquaresX = (xMax - xMin) / gridSizeX;
    float distanceBetweenSquaresZ = (zMax - zMin) / gridSizeZ;

    gsl::Vector3D planeGrid[gridSizeX][gridSizeZ]; // planeGrid stores the x,z and an average y height for all points in the data
    int pointDataOutOfGrid = pointData.size(); //Used to calculate how many points are missing
    int nrPoints[gridSizeX][gridSizeZ] = {{0}}; //Used to count how many points are in each square
    float sumPointData[gridSizeX][gridSizeZ] = {{0}}; //Used to sum all the points in each square, is then used to average the y.

//    MeshData MDPoints;

    qDebug() << "planeGrid is being filled with data"; //Used to output some progress in application output.

for (int x = 0; x < gridSizeX; x++)
{
    for (int z = 0; z < gridSizeZ; z++)
    {
        //Fill the array with evenly spaced coordinates "enclosing" all pointData positions
        planeGrid[x][z].x = xMin + (distanceBetweenSquaresX * x);
        planeGrid[x][z].z = zMin + (distanceBetweenSquaresZ * z);

        //Check how many points there are between each position in the grid using resolution
        for (int pointDataSearch = 0; pointDataSearch < pointData.size(); pointDataSearch++) //This becomes a huge for-loop, trying to get out of it with the data i need.
        {
            if(pointData[pointDataSearch].getX() >= planeGrid[x][z].x && //If a pointData coordinate is bigger then current planeGrid position
               pointData[pointDataSearch].getZ() >= planeGrid[x][z].z &&
               pointData[pointDataSearch].getX() < (planeGrid[x][z].x + distanceBetweenSquaresX) && //But also needs to be before next planeGrid square.
               pointData[pointDataSearch].getZ() < (planeGrid[x][z].z + distanceBetweenSquaresZ))
            {
                pointDataOutOfGrid--; // Keep track of missing points.

                nrPoints[x][z]++;
                sumPointData[x][z] += pointData[pointDataSearch].getY();
            }
        }
        //This is now in the for loop running for x * z

        if(nrPoints[x][z] != 0) //There might be some squares with 0 points, this makes sure we dont divide by zero
        {
            planeGrid[x][z].y = sumPointData[x][z]/nrPoints[x][z]; //planeGrid is now filled with a 50x50 square with x,z and average of y coordinates.
        }

        //After having found all y-cordinates, subtract min in each coordinate, to get origin at 0
        planeGrid[x][z].x -= xMin; //This should make the origin of the datapoints at 0 in scene.
        planeGrid[x][z].z -= zMin;
//        planeGrid[x][z].y -= yMin; //Might not be needed, have other options for scaling y height

        //Print out all points as openGL_Points
        /*MDPoints = */makePoint((planeGrid[x][z]/*.x, planeGrid[x][z].y, planeGrid[x][z].z*/), 1.0f, (static_cast<void>(0.0f), 1.0f, 0.0f)); //Print the point with cordinates, size 1, and green color
//        glVertex3f(planeGrid[x][z].x, planeGrid[x][z].y, planeGrid[x][z].z);
    }
}

//    mGameObjectManager->makePointObject(MDPoints);


    qDebug() << "planeGrid is now filled"; //As of 26.11-2021 this takes about half a minute on my computer.
    qDebug() << "Total pointData" << pointData.size() << "Point data not found in search" << pointDataOutOfGrid;
//    qDebug() << "Nr of points in square [0][39] is" << nrPoints[0][39] << "their total height" << sumPointData[0][39];
//    qDebug() << "Planegrid at square [0][39]" << planeGrid[0][39];
//    qDebug() << "xMin" << xMin << "yMin" << yMin << "zMin" << zMin;
//    qDebug() << "After the for loops" << "x" << planeGrid[0][0].x << "y" << planeGrid[0][0].y << "z" << planeGrid[0][0].z;


    float widthSize = 25.0f; //How big the mesh-width will be in scene 1 = normal, higher number = smaller
    float depthSize = 25.0f; //How big the mesh-depth will be in scene 1 = normal, higher number = smaller
    float heigthScale = 25.0f; //How "flat" the surface will be, 1 = big difference, 100 = "flatter"

    int c = 0;
//    qDebug() << "Start of triangle creation";
    for (int width = 0; width > gridSizeX; width++)
    {
        for (int depth = 0; depth > gridSizeZ; depth++)
        {
//            float y = planeGrid[width][depth].y / heigthScale;

            //Check to avoid drawing a triangle from one side of the terrain all the way over to the other side.
            if(c == depth-1)
            {
                c=0;
                continue;
            }
            c++;

            //Create a triangle using the points in this order.
//            mMeshes.mIndices[0].push_back(width);   // But mIndeces is a member of meshdata?
//            mMeshes.mIndices[0].push_back(width+1); // mMeshes is a vector of MeshData, and the struct MeshData contains mIndeces?
//            mMeshes.mIndices[0].push_back(width+depth);

//            mMeshes.mIndices[0].push_back(width+depth);
//            mMeshes.mIndices[0].push_back(width+1);
//            mMeshes.mIndices[0].push_back(width+depth+1);
        }
    }
//    qDebug() << "End of triangle calculation";


    //Normal calculation
    gsl::Vector3D pCenter,p0,p1,p2,p3,p4,p5; //Points
    gsl::Vector3D n0,n1,n2,n3,n4,n5;         //Normals

//    std::vector<Vertex> vert = object->mesh->mVertices[0];

//    for(int i = 0; i < gridSizeX * gridSizeZ; i++)
//    {
////        gsl::Vector3D pos = object->mesh->mVertices[0][i].mXYZ;

//        int x1 = static_cast<int>((pos.x) / xStep);
//        int y1 = static_cast<int>((pos.z) / zStep);

//        if(x1 > 0 && y1 > 0 && x1 < rows && y1 < cols) //index > 0
//        {
//            pCenter = gsl::Vector3D{pos.x, pos.y, pos.z};

//            p0 = gsl::Vector3D{vert[i-rows].mXYZ.x, vert[i-rows].mXYZ.y, vert[i-rows].mXYZ.z};
//            p1 = gsl::Vector3D{vert[i+1-rows].mXYZ.x, vert[i+1-rows].mXYZ.y, vert[i+1-rows].mXYZ.z};
//            p2 = gsl::Vector3D{vert[i+1].mXYZ.x, vert[i+1].mXYZ.y, vert[i+1].mXYZ.z};
//            p3 = gsl::Vector3D{vert[i+rows].mXYZ.x, vert[i+rows].mXYZ.y, vert[i+rows].mXYZ.z};
//            p4 = gsl::Vector3D{vert[i-1+rows].mXYZ.x, vert[i-1+rows].mXYZ.y, vert[i-1+rows].mXYZ.z};
//            p5 = gsl::Vector3D{vert[i-1].mXYZ.x, vert[i-1].mXYZ.y, vert[i-1].mXYZ.z};
//        }

//        //Lager vektorer til alle punktene
//        gsl::Vector3D v0 = p0-pCenter;
//        gsl::Vector3D v1 = p1-pCenter;
//        gsl::Vector3D v2 = p2-pCenter;
//        gsl::Vector3D v3 = p3-pCenter;
//        gsl::Vector3D v4 = p4-pCenter;
//        gsl::Vector3D v5 = p5-pCenter;

//    /** If i get the absolute values the terrain gets lit. If not, it's dark (Has negative values) */
//        v0 = gsl::Vector3D{std::abs(v0.x), std::abs(v0.y), std::abs(v0.z)};
//        v1 = gsl::Vector3D{std::abs(v1.x), std::abs(v1.y), std::abs(v1.z)};
//        v2 = gsl::Vector3D{std::abs(v2.x), std::abs(v2.y), std::abs(v2.z)};
//        v3 = gsl::Vector3D{std::abs(v3.x), std::abs(v3.y), std::abs(v3.z)};
//        v4 = gsl::Vector3D{std::abs(v4.x), std::abs(v4.y), std::abs(v4.z)};
//        v5 = gsl::Vector3D{std::abs(v5.x), std::abs(v5.y), std::abs(v5.z)};

//        //Regner ut normalene til alle trekantene rundt punktet

//        n0 = v0.cross(v0,v1);
//        n0.normalize();

//        n1 = v1.cross(v1,v2);
//        n1.normalize();

//        n2 = v2.cross(v2,v3);
//        n2.normalize();

//        n3 = v3.cross(v3,v4);
//        n3.normalize();

//        n4 = v4.cross(v4,v5);
//        n4.normalize();

//        n5 = v5.cross(v5,v0);
//        n5.normalize();

//        gsl::Vector3D nV = n0+n1+n2+n3+n4+n5;

//        object->mesh->mVertices[0].at(i).set_normal(nV.x, nV.y, nV.z);
//    }
}


void MeshHandler::initMesh(MeshData &tempMesh, int lodLevel)
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &tempMesh.mVAO[lodLevel] );
    glBindVertexArray( tempMesh.mVAO[lodLevel] );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &tempMesh.mVBO[lodLevel] );
    glBindBuffer( GL_ARRAY_BUFFER, tempMesh.mVBO[lodLevel] );

    glBufferData( GL_ARRAY_BUFFER, tempMesh.mVertices[lodLevel].size()*sizeof(Vertex),
                  tempMesh.mVertices[lodLevel].data(), GL_STATIC_DRAW );

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
    if(tempMesh.mIndices[lodLevel].size() > 0) {
        glGenBuffers(1, &tempMesh.mEAB[lodLevel]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tempMesh.mEAB[lodLevel]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, tempMesh.mIndices[lodLevel].size() * sizeof(GLuint),
                     tempMesh.mIndices[lodLevel].data(), GL_STATIC_DRAW);
    }

    glBindVertexArray(0);

    tempMesh.mIndexCount[lodLevel] = tempMesh.mIndices[lodLevel].size();
    tempMesh.mVertexCount[lodLevel] = tempMesh.mVertices[lodLevel].size();
}


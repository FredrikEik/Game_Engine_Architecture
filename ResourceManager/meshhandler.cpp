#include "ResourceManager/meshhandler.h"

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

MeshHandler &MeshHandler::getInstance()
{
    static MeshHandler *mInstance = new MeshHandler();
    return *mInstance;
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
        if (meshName.find("Data") != std::string::npos)
            meshIndex = readLasFile();


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
        //Closing the file after use
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

int MeshHandler::readLasFile()
{
    float xMin{0}, xMax{0}, yMin{0}, yMax{0}, zMin{0}, zMax{0};

    std::ifstream inLasFile;
    inLasFile.open(gsl::ProjectFolderName + "Assets/data.txt"); //Open file using a constant adress

    if(!inLasFile) //if file not found
    {
        qDebug() << "unable to open .las file";
        exit(EXIT_FAILURE);
    }

    std::vector<gsl::Vector3D> allLasPointData; //Used to store all data from file
    int sizeOfLasFile = {0}; //Used to store the first line in the document, its an int with the number of lines in the las file.
    float lasX, lasY, lasZ = {0}; //Temp variables overwritten every line, used to fill Vector3D pointData.

    if(inLasFile.is_open())
    {
        qDebug() << "Opening las file"; //Sort of timer functionality, lets me know in application output if something freezes.
        inLasFile >> sizeOfLasFile; //read first line, the approximate number of datalines.
        allLasPointData.reserve(sizeOfLasFile); //reserve the space for the number of datalines.

        for(int i = 0; i < sizeOfLasFile; i++) //for each line of data fill pointData with numbers
        {
            inLasFile >> lasX >> lasZ >> lasY; //In the Las file the data is stored x,z,y
            allLasPointData.emplace_back(lasX, lasY, lasZ); //I fill the pointdata with x,y,z.
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


    //--------------Done reading las file, on to creating a simplification-------------------//


    // This serves as "resolution" for planeGrid[arrayX][arrayZ].
    const int arrayX = 6; //Using 6x6 to get 5x5m squares.
    const int arrayZ = 6;

    float widthScale  = 1.0f; //How big the mesh-width will be in WorldSpace 1 = normal, higher number = smaller
    float depthScale  = 1.0; //How big the mesh-depth will be in WorldSpace 1 = normal, higher number = smaller (Data is slighly rectangular, not square, so to make it more square, scale is different)
    float heigthScale = 1.0f; //How "flat" the surface will be, 1 = normal, higher number = smaller difference

    //The assignment specified a 5x5 meter grid.
    float distanceBetweenSquaresX = 5.0f;//(xMax - xMin) / arrayX;
    float distanceBetweenSquaresZ = 5.0f;//(zMax - zMin) / arrayZ;

    //Various variables needed for the simplification
    gsl::Vector3D planeGrid [arrayX][arrayZ] = {{0}}; // planeGrid stores a rectangulated x, z and an average y height for all points in the data
    int nrPoints            [arrayX][arrayZ] = {{0}}; // Used to count how many points are in each square
    float sumPointData      [arrayX][arrayZ] = {{0}}; // Used to sum all the points in each square, is then used to average the y height.

    //Getting a reference of the last created mesh, should be "empty" beacuse it was recently created and filled below.
    mMeshes.emplace_back(MeshData());
    MeshData &meshDataPoints = mMeshes.back(); //Used to store scaled and positioned correctly vector3d's

    qDebug() << "planeGrid is being filled with data of size" << arrayX << "*" << arrayZ << ". The higher the number, the longer time needed. Most likely not crashed"; //Used to output some progress in application output.
    qDebug() << "For example 50 * 50 gets me ok resolution, and takes about 30 seconds to compute on my machines in debug mode";

    for (int x = 0; x < arrayX; x++)
    {
        for (int z = 0; z < arrayZ; z++)
        {
            //Fill the array with evenly spaced coordinates "enclosing" all pointData positions
            planeGrid[x][z].x = xMin + (distanceBetweenSquaresX * x);
            planeGrid[x][z].z = zMin + (distanceBetweenSquaresZ * z);
            planeGrid[x][z].y = yMin;

            //Check how many points there are between each position in the grid using resolution
            for (int pointDataSearch = 0; pointDataSearch < allLasPointData.size(); pointDataSearch++) //This becomes a huge for-loop, trying to get out of it with the data i need.
            {
                if(allLasPointData[pointDataSearch].getX() >= planeGrid[x][z].x && //If a pointData coordinate is bigger then current planeGrid position
                   allLasPointData[pointDataSearch].getZ() >= planeGrid[x][z].z &&
                   allLasPointData[pointDataSearch].getX() < (planeGrid[x][z].x + distanceBetweenSquaresX) && //But also needs to be before next planeGrid square.
                   allLasPointData[pointDataSearch].getZ() < (planeGrid[x][z].z + distanceBetweenSquaresZ))
                {
                    nrPoints[x][z]++; // Keep track of how many y positions in spesific x z squares.
                    sumPointData[x][z] += allLasPointData[pointDataSearch].getY(); //Sum all the y positions in x z, used to average.
                }
            }

            ////This is now in the for loop running for x * z
            if(nrPoints[x][z] != 0) //if points are found, create an average height for the point.
            {
                planeGrid[x][z].y = sumPointData[x][z] / nrPoints[x][z]; //If one or more point is found, make an average value.
            }

            planeGrid[x][z].x -= xMin; //Gives new positions to points, to give a resonable origin of the mesh in scene
            planeGrid[x][z].z -= zMin;
            planeGrid[x][z].y -= yMin;

            planeGrid[x][z].x /= widthScale; //After all points found, scale the distance between points, to give a resonable size of the mesh in scene
            planeGrid[x][z].z /= depthScale;
//            planeGrid[x][z].y /= heigthScale; //HeightScale not needed, beacuse using yMin is resonable.

            //Only using mVertices[0] beacuse there is no lod for this mesh.
            //It could have been fun to use the "resolution" or arrayX & arrayY to produce lower quality LODs, but that falls a bit out of scope for Vis & Sim.
            meshDataPoints.mVertices[0].emplace_back(Vertex{planeGrid[x][z].x, planeGrid[x][z].y, planeGrid[x][z].z, //Positions
                                                            0.0f, 0.0f, 0.0f, //Normals
                                                            0.0f, 0.0f});     //UVs
        }
    }
    qDebug() << "planeGrid is now filled"; //Basic progress outputting. Let the user know the program hasnt crashed.

    ////Create triangle based on the points
    qDebug() << "Start of triangle calculations (should be much less expensive)";

    int check = 0;
    for (int depth = 0; depth < (meshDataPoints.mVertices[0].size() - arrayX - 1); depth++) //This should run for all the rows of the grid, but stop before "overdrawing"
    {
        //Check to avoid drawing a triangle from one side of the terrain all the way over to the other side.
        if(check == arrayX - 1)
        {
            check = 0;
            continue;
        }
        check++;

        //Create two triangles using the vertices at these positions of mVertices. Only filling first LOD
        meshDataPoints.mIndices[0].push_back(depth);
        meshDataPoints.mIndices[0].push_back(depth + 1);
        meshDataPoints.mIndices[0].push_back(depth + arrayX);

        meshDataPoints.mIndices[0].push_back(depth + arrayX);
        meshDataPoints.mIndices[0].push_back(depth + 1);
        meshDataPoints.mIndices[0].push_back(depth + arrayX + 1);
    }
    qDebug() << "End of triangle calculation";


    qDebug() << "Start of normal calculation";
{
////Leaving some variations that i implemented, that could have worked, but did not.
//    gsl::Vector3D triangleNormal{0};
//    int count = 0;

//    for(int i = 0; i < meshDataPoints.mVertices[0].size() - 2; i +=3)
//    {
//        count++;
        ////Calculating triangle normals variant 1 (Used in phsycicshandler)
//        triangleNormal = (meshDataPoints.mVertices[0][i+1].mXYZ - meshDataPoints.mVertices[0][i].mXYZ) ^
//                         (meshDataPoints.mVertices[0][i+1].mXYZ - meshDataPoints.mVertices[0][i+2].mXYZ);
//    //            qDebug() << "Triangle normal is:" << triangleNormal;
//        triangleNormal.normalize();
//    //            qDebug() << "Normalized Triangle is:" << triangleNormal;
//        meshDataPoints.mVertices[0][i].set_normal(triangleNormal);

        ////Calculating triangle normals variant 2 - https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal
        ///"Pseudo-code" example
//        gsl::Vector3D U = meshDataPoints.mVertices[0][i+1].mXYZ - meshDataPoints.mVertices[0][i].mXYZ;
//        gsl::Vector3D V = meshDataPoints.mVertices[0][i+2].mXYZ - meshDataPoints.mVertices[0][i].mXYZ;

//        triangleNormal.x = (U.y * V.z) - (U.z * V.y);
//        triangleNormal.y = (U.z * V.x) - (U.x * V.z);
//        triangleNormal.z = (U.x * V.y) - (U.y * V.x);

//        meshDataPoints.mVertices[0][i].set_normal(triangleNormal);
//    }
//    qDebug() << "Number of triangle normals calculated" << count;
}
    ////Calculating triangle normals variant 3 (original) and so far best
    gsl::Vector3D pCenter,p0,p1,p2,p3,p4,p5; //Points
    gsl::Vector3D n0,n1,n2,n3,n4,n5;         //Normals

    for (int i = 1; i < (meshDataPoints.mVertices[0].size() - arrayX); i++) // -arrayX to not use triangles out of "bounds".
    {
        gsl::Vector3D pos = meshDataPoints.mVertices[0][i].mXYZ; //Get the position of mVertice nr in LOD 0.

        if(pos.x > 0 && pos.z > 0)
        {
            pCenter = pos; //Store the position of the point we are currently on

//            qDebug() << "Normal calculated" << i;

            // Store the nearest vertices, based on how the triangles been created.
            p0 = meshDataPoints.mVertices[0][i - arrayX    ].mXYZ;
            p1 = meshDataPoints.mVertices[0][i + 1 - arrayX].mXYZ;
            p2 = meshDataPoints.mVertices[0][i + 1         ].mXYZ;
            p3 = meshDataPoints.mVertices[0][i + arrayX    ].mXYZ;
            p4 = meshDataPoints.mVertices[0][i - 1 + arrayX].mXYZ;
            p5 = meshDataPoints.mVertices[0][i - 1         ].mXYZ;

        }

    //Create a vector to all the points
    gsl::Vector3D v0 = p0 - pCenter;
    gsl::Vector3D v1 = p1 - pCenter;
    gsl::Vector3D v2 = p2 - pCenter;
    gsl::Vector3D v3 = p3 - pCenter;
    gsl::Vector3D v4 = p4 - pCenter;
    gsl::Vector3D v5 = p5 - pCenter;

    //Calulate the normals for each triangle around the point
    n0 = v0.cross(v0, v1);
    n0.normalize();

    n1 = v1.cross(v1, v2);
    n1.normalize();

    n2 = v2.cross(v2, v3);
    n2.normalize();

    n3 = v3.cross(v3, v4);
    n3.normalize();

    n4 = v4.cross(v4, v5);
    n4.normalize();

    n5 = v5.cross(v5, v0);
    n5.normalize();

    gsl::Vector3D nV = n0+n1+n2+n3+n4+n5;

    meshDataPoints.mVertices[0][i].set_normal(nV);
}
qDebug() << "End of normal calculations";

//Finalize mesh
//meshDataPoints.mDrawType = GL_POINTS; //If points are needed instead of default triangles.
//glPointSize(5.0f);
initMesh(meshDataPoints, 0);
return mMeshes.size()-1;
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


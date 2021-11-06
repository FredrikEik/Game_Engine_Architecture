#include "meshhandler.h"

#include <sstream>
#include <fstream>

#include "constants.h"
#include "math_constants.h"
#include "camera.h"
#include "logger.h"
#include "texturehandler.h"

MeshHandler::MeshHandler()
{
    //This is a hack - to make sure my MeshComponens are not moved for now
    //Please fix
    mMeshes.reserve(gsl::MAX_MESHCOMPONENTS);
    mLogger = Logger::getInstance();    //Have to do this, else program will crash
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
        if (meshName.find("terrain") != std::string::npos)
            meshIndex = makeTerrain("terrain.bmp");


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
        mLogger->logText("Reading " + tempName);
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
                    makeColliderData(temp, tempVertex);

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

int MeshHandler::makeTerrain(std::string heightMapName)
{
    //should check if this object is new before this!
    mMeshes.emplace_back(MeshData());
    MeshData &tempMesh = mMeshes.back();

    //getting the heightmap texture for the terrain
    TextureHandler* textureHandler = TextureHandler::getInstance();
    int textureIndex =  textureHandler->mTextureMap.find(heightMapName)->second;
    Texture &heighMap = textureHandler->mTextures.at(textureIndex);

    //Default normal pointing straight up - should be calculated correctly for lights to work!!!
    gsl::Vector3D normal{0.f, 1.f, 0.f};

    //how many meters between each vertex in both x and z direction
    float horisontalSpacing{.5f};
    //scaling the height read from the heightmap. 0 + 255 meters if this is set to 1
    float heightSpacing{.02f};
     //offset the whole terrain in y (height) axis
    float heightPlacement{-5.f};

    //Getting the scale of the heightmap
    unsigned short width = heighMap.mColumns;   //Width - x-axis
    unsigned short depth = heighMap.mRows;      //Depth - z-axis

    //temp variables for creating the mesh
    float vertexXStart{0.f - width*horisontalSpacing/2};
    float vertexZStart{0.f + depth*horisontalSpacing/2};

    //loop making the mesh from the values read from the heightmap
    for(int d{0}; d < depth; ++d)
    {
        for(int w{0}; w < width; ++w)
        {
            //heightmap is actually stored as an one dimentional array - so calculating the correct index for column and row
            //and scale it according to variables
            float heightFromBitmap = heighMap.getHeightFromIndex(w + d*depth) * heightSpacing + heightPlacement;
            tempMesh.mVertices[0].emplace_back(gsl::Vector3D{vertexXStart + (w * horisontalSpacing), heightFromBitmap,
                                                             vertexZStart - (d * horisontalSpacing)}, normal, gsl::Vector2D{w / (width-1.f), d / (depth-1.f)});
            //Making bounding box - not tested if correct
            if(heightFromBitmap < tempMesh.mLowLeftBackCorner.y)
                tempMesh.mLowLeftBackCorner.y = heightFromBitmap;
            if(heightFromBitmap > tempMesh.mUpRightFrontCorner.y)
                tempMesh.mUpRightFrontCorner.y = heightFromBitmap;
        }
    }

    // The mesh(grid) is drawn in quads with diagonals from lower left to upper right
    //          _ _
    //         |/|/|
    //          - -
    //         |/|/|
    //          - -
    //Making the indices for this mesh:
    for(int d{0}; d < depth-1; ++d)        //depth - 1 because we draw the last quad from depth - 1 and in negative z direction
    {
        for(int w{0}; w < width-1; ++w)    //width - 1 because we draw the last quad from width - 1 and in positive x direction
        {
            tempMesh.mIndices[0].emplace_back(w + d * width);               // 0 + 0 * mWidth               = 0
            tempMesh.mIndices[0].emplace_back(w + d * width + width + 1);   // 0 + 0 * mWidth + mWidth + 1  = mWidth + 1
            tempMesh.mIndices[0].emplace_back(w + d * width + width);       // 0 + 0 * mWidth + mWidth      = mWidth
            tempMesh.mIndices[0].emplace_back(w + d * width);               // 0 + 0 * mWidth               = 0
            tempMesh.mIndices[0].emplace_back(w + d * width + 1);           // 0 + 0 * mWidth + 1           = 1
            tempMesh.mIndices[0].emplace_back(w + d * width + width + 1);   // 0 + 0 * mWidth + mWidth + 1  = mWidth + 1
        }
    }

    float totalWidthMeters = (width - 1) * horisontalSpacing;

    Logger::getInstance()->logText("Ground made | Total Width in Meters " + std::to_string(totalWidthMeters) + "| Size: " + std::to_string(width) + ", " +
                    std::to_string(depth) + "| VertexXStart:" + std::to_string(vertexXStart) + "| VertexZStart" + std::to_string(vertexZStart));

    calculateHeighMapNormals(width, depth, tempMesh);

    //only LOD level 0
    initMesh(tempMesh, 0);

    return mMeshes.size()-1;    //returns index to last object
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
    MeshData tempMesh;
    float xMinus{lowLeftBack.x};
    float xPlus{upRightFront.x};
    float yMinus{lowLeftBack.y};
    float yPlus{upRightFront.y};
    float zMinus{lowLeftBack.z};
    float zPlus{upRightFront.z};

    tempMesh.mVertices[0].insert( tempMesh.mVertices[0].end(),
      {//Vertex data for front points       color                       uv
       Vertex{xMinus, yMinus, zPlus,       1.f, 0.301f, 0.933f,          0.f, 0.f},     // 0
       Vertex{xPlus,  yMinus, zPlus,       1.f, 0.301f, 0.933f,          0.f, 0.f},
       Vertex{xPlus,  yPlus,  zPlus,       1.f, 0.301f, 0.933f,          0.f, 0.f},
       Vertex{xMinus, yPlus,  zPlus,       1.f, 0.301f, 0.933f,          0.f, 0.f},
       //Vertex data for back
       Vertex{xMinus, yMinus, zMinus,      1.f, 0.301f, 0.933f,          0.f, 0.f},    // 4
       Vertex{xPlus,  yMinus, zMinus,      1.f, 0.301f, 0.933f,          0.f, 0.f},
       Vertex{xPlus,  yPlus,  zMinus,      1.f, 0.301f, 0.933f,          0.f, 0.f},
       Vertex{xMinus, yPlus,  zMinus,      1.f, 0.301f, 0.933f,          0.f, 0.f},
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

void MeshHandler::makeColliderData(MeshData &meshIn, gsl::Vector3D &vertexIn)
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

    // 1st attribute buffer : position
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

void MeshHandler::calculateHeighMapNormals(int width, int depth, MeshData &mesh)
{
    //TODO: This can be optimized and calculated while heightMap is read in

    //not tested for non square textures:
    if (width != depth)
    {
        Logger::getInstance()->logText("Normals for ground not calclulated! Use square texture!");
        return;
    }

    unsigned short resolutionSquared= width * depth;

    //terrain has the diagonal of the quads pointing up and right, like |/| (not |\|)

    //************************* Special case - bottom row has no vertices below **********************************
    for(unsigned short i{0}; i < width; i++)
    {
        //temporary normals for each of the 6 surrounding triangles
        gsl::Vector3D surroundingNormals[6]{};

        //the center vertex that we calculate the normal for
        gsl::Vector3D center = mesh.mVertices[0].at(i).mXYZ;

        gsl::Vector3D first{};  //first vector will be crossed with
        gsl::Vector3D second{}; //second vector - check right hand rule!

        // a: first = north, second = west
        //check if we are to the left, if so - skip:
        //If first vertex - skip - already (0, 1, 0)
        if(i != 0)
        {
            first = mesh.mVertices[0].at(i+ width).mXYZ - center;
            second = mesh.mVertices[0].at(i-1).mXYZ - center;
            surroundingNormals[0] = first ^ second;
        }
        // b: first and second flips so we don't have to calculate a vector we already have!!
        // second = north east, first = north
        //check if we are at left, if so - skip
        if( ((i + 1) % width) == false)
        {
            second = mesh.mVertices[0].at(i + width + 1).mXYZ - center;
            //first = mVertices.at(i+ mWidth).mXYZ - center;
            surroundingNormals[1] = second ^ first;

            //c: first = east, second = north east
            first = mesh.mVertices[0].at(i+1).mXYZ - center;
            //second = mVertices.at(i + mWidth + 1).mXYZ - center;
            surroundingNormals[2] = first ^ second;
        }

        //add all vectors and normalize
        gsl::Vector3D result{};
        for(int i{0}; i < 6; i++)
        {
            surroundingNormals[i].normalize();
            result += surroundingNormals[i];
        }
        result.normalize();

        //put the normal into the vertex
//        qDebug() << result;
        mesh.mVertices[0].at(i).mNormal = result;
    }

    //calculate each of the triangles normal - omitting the outer vertices
    //starting av mWidth + 1 == the second vertex on the second row
    //ending each row at second to last vertex, to not hit the outer edge
    //ending at resolutionSquared - mWidth - 2 == second to last vertex on the second to last row
    for(unsigned short i = width + 1 ; i < resolutionSquared - width - 2; i++)
    {
        //if at the end of a row, jump to next
        if( (i + 2) % width == 0)
        {   i += 2; //have to add 2 to get to the next correct index
            continue;
        }
        //goes in a counter clockwise direction
        //terrain has the diagonal of the quads pointing up and right, like |/| (not |\|)

        //temporary normals for each of the 6 surrounding triangles
        gsl::Vector3D surroundingNormals[6]{};

        //the center vertex that we calculate the normal for
        gsl::Vector3D center = mesh.mVertices[0].at(i).mXYZ;

        //first and second vector flips each time because second
        //is the "first" at the next calculation.
        //We don't want to calculate again!
        gsl::Vector3D first{};
        gsl::Vector3D second{};

        // a 0: first = north, second = west
        //check if we are to the left, if so - skip:
        first = mesh.mVertices[0].at(i+ width).mXYZ - center;
        second = mesh.mVertices[0].at(i-1).mXYZ - center;
        surroundingNormals[0] = first ^ second;

        // b 1: first = south west, second = west
        first = mesh.mVertices[0].at(i - width - 1).mXYZ - center;
        surroundingNormals[1] = second ^ first;

        //c 2: first = south west, second = south
        second = mesh.mVertices[0].at(i - width).mXYZ - center;
        surroundingNormals[2] = first ^ second;

        //d 3: second = south, first = east
        //check if we are to the left, if so - skip
        first = mesh.mVertices[0].at(i + 1).mXYZ - center;
        surroundingNormals[3] = second ^ first;

        //e 4: first = east, second = north east
        second = mesh.mVertices[0].at(i + width +1).mXYZ - center;
        surroundingNormals[4] = first ^ second;

        //f 5: second = north east, first = north
        first = mesh.mVertices[0].at(i + width).mXYZ - center;
        surroundingNormals[5] = second ^ first;

        //add all vectors and normalize
        gsl::Vector3D result{};
        for(int i{0}; i < 6; i++)
        {
            surroundingNormals[i].normalize();
            result += surroundingNormals[i];
        }
        result.normalize();

        //put the normal into the vertex
//        qDebug() << result;
        mesh.mVertices[0].at(i).mNormal = result;
    }
    Logger::getInstance()->logText("Normals for ground calclulated", LColor::HIGHLIGHT);
}


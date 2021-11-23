#include "meshhandler.h"

#include <sstream>
#include <fstream>
#include "vertex.h"
#include "constants.h"
#include "math_constants.h"
#include <random>
#include <stdlib.h>
#include "coreengine.h"
#include "texturehandler.h"
#include "camera.h"

MeshHandler::MeshHandler()
{
    //This is a hack - to make sure my MeshComponens are not moved for now
    //Please fix
    mMeshes.reserve(gsl::MAX_MESHCOMPONENTS);
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
        if (meshName.find("cube") != std::string::npos)
            meshIndex = makeCube();
        if (meshName.find("terrain") != std::string::npos)
            meshIndex = makeTerrain("heightmap.bmp");
        if (meshName.find("particle") != std::string::npos)
            meshIndex = makeParticle();
        if (meshName.find("projectile") != std::string::npos)
            meshIndex = makeProjectile();
        if (meshName.find("skybox") != std::string::npos)
            meshIndex = makeSkybox();
        if (meshName.find("LAS") != std::string::npos)
            meshIndex = makeLAS("test_las.txt");

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

    // testing stuff ///////


    //tempName = gsl::MeshFilePath + tempName + ".obj";
    //  ^^^^^¨¨^^^^^     /////////
    for (unsigned short lod{0}; lod < 3; lod++ )  //we test for 3 LOD levels
    {
        if (lod == 0)     //original mesh - not reduced size
            tempName = filename;
        else
            tempName = filename + "_L0" + std::to_string(lod);

        tempName = gsl::MeshFilePath + tempName + ".obj";
        qDebug() << "Reading " << tempName.c_str();
        std::ifstream fileIn;
        fileIn.open (tempName, std::ifstream::in);
        if(!fileIn)
        {
            qDebug() << "ERROR: Could not open file for reading: " << QString::fromStdString(filename);
            qDebug() << "****** using arbitrary mesh as replacement!";
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

    //calculate ca radius for collider - TODO: this is not correct:
    float a = temp.mLowLeftBackCorner.length();
    float b = temp.mUpRightFrontCorner.length();
    temp.mColliderRadius = (a>b) ? a : b;

    qDebug() << QString::fromStdString(filename) << "successfully loaded";

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

    //only LOD level 0
    initMesh(temp, 0);

    return mMeshes.size()-1;    //returns index to last object
}


int MeshHandler::makeParticle()
{
    mMeshes.emplace_back(MeshData());
    MeshData &temp = mMeshes.back();

    temp.mVertices[0].push_back(Vertex(-0.5,-0.5,0.5   , 1, 0, 0));
    temp.mVertices[0].push_back(Vertex(0.5,-0.5,0.5    , 1, 0, 0));
    temp.mVertices[0].push_back(Vertex(0.5,0.5,0.5     , 1, 0, 0));

    temp.mVertices[0].push_back(Vertex(-0.5,-0.5,0.5   , 1, 0, 0));
    temp.mVertices[0].push_back(Vertex(0.5,0.5,0.5     , 1, 0, 0));
    temp.mVertices[0].push_back(Vertex(-0.5,0.5,0.5    , 1, 0, 0));

    temp.mDrawType = GL_TRIANGLES;
    initMesh(temp, 0);
    return mMeshes.size()-1;
}

int MeshHandler::makeProjectile()
{
    mMeshes.emplace_back(MeshData());
    MeshData &tempMesh = mMeshes.back();
float radius = .1;
bool rgbColor = true;

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

    tempMesh.mDrawType = GL_TRIANGLES;

    //only LOD level 0
    initMesh(tempMesh, 0);

    return mMeshes.size()-1;
}

int MeshHandler::makeSkybox()
{
    mMeshes.emplace_back(MeshData());
    MeshData &temp = mMeshes.back();

    temp.mVertices[0].insert( temp.mVertices[0].end(),
    {// front
     Vertex{gsl::Vector3D(-1.f, -1.f, 1.f),    gsl::Vector3D(0.f, 0.f, 1.0f),  gsl::Vector2D(0.25f, 0.333f)},
     Vertex{gsl::Vector3D( 1.f, -1.f, 1.f),    gsl::Vector3D(0.f, 0.f, 1.0f),  gsl::Vector2D(0.5f,  0.333f)},
     Vertex{gsl::Vector3D(-1.f,  1.f, 1.f),    gsl::Vector3D(0.f, 0.f, 1.0f),  gsl::Vector2D(0.25f, 0.666f)},
     Vertex{gsl::Vector3D( 1.f,  1.f, 1.f),    gsl::Vector3D(0.f, 0.f, 1.0f),  gsl::Vector2D(0.5f,  0.666f)},

     // right
     Vertex{gsl::Vector3D(1.f, -1.f,  1.f),    gsl::Vector3D(1.f, 0.f, 0.f),   gsl::Vector2D(0.5f,  0.333f)},
     Vertex{gsl::Vector3D(1.f, -1.f, -1.f),    gsl::Vector3D(1.f, 0.f, 0.f),   gsl::Vector2D(0.75f, 0.333f)},
     Vertex{gsl::Vector3D(1.f,  1.f,  1.f),    gsl::Vector3D(1.f, 0.f, 0.f),   gsl::Vector2D(0.5f,  0.666f)},
     Vertex{gsl::Vector3D(1.f,  1.f, -1.f),    gsl::Vector3D(1.f, 0.f, 0.f),   gsl::Vector2D(0.75f, 0.666f)},

     // back
     Vertex{gsl::Vector3D( 1.f, -1.f, -1.f),   gsl::Vector3D(0.f, 0.f, -1.f),  gsl::Vector2D(0.75f, 0.333f)},
     Vertex{gsl::Vector3D(-1.f, -1.f, -1.f),   gsl::Vector3D(0.f, 0.f, -1.f),  gsl::Vector2D(1.f,   0.333f)},
     Vertex{gsl::Vector3D( 1.f,  1.f, -1.f),   gsl::Vector3D(0.f, 0.f, -1.f),  gsl::Vector2D(0.75f, 0.666f)},
     Vertex{gsl::Vector3D(-1.f,  1.f, -1.f),   gsl::Vector3D(0.f, 0.f, -1.f),  gsl::Vector2D(1.f,   0.666f)},

     // left
     Vertex{gsl::Vector3D(-1.f, -1.f, -1.f),   gsl::Vector3D(-1.f, 0.f, 0.f),  gsl::Vector2D(0.f, 0.333f)},
     Vertex{gsl::Vector3D(-1.f, -1.f,  1.f),   gsl::Vector3D(-1.f, 0.f, 0.f),  gsl::Vector2D(0.25f, 0.333f)},
     Vertex{gsl::Vector3D(-1.f,  1.f, -1.f),   gsl::Vector3D(-1.f, 0.f, 0.f),  gsl::Vector2D(0.f, 0.666f)},
     Vertex{gsl::Vector3D(-1.f,  1.f,  1.f),   gsl::Vector3D(-1.f, 0.f, 0.f),  gsl::Vector2D(0.25f,   0.666f)},

     // bottom
     Vertex{gsl::Vector3D(-1.f, -1.f, -1.f),   gsl::Vector3D(0.f, -1.f, 0.f),  gsl::Vector2D(0.25f, 0.f)},
     Vertex{gsl::Vector3D( 1.f, -1.f, -1.f),   gsl::Vector3D(0.f, -1.f, 0.f),  gsl::Vector2D(0.5f,  0.f)},
     Vertex{gsl::Vector3D(-1.f, -1.f,  1.f),   gsl::Vector3D(0.f, -1.f, 0.f),  gsl::Vector2D(0.25f, 0.333f)},
     Vertex{gsl::Vector3D( 1.f, -1.f,  1.f),   gsl::Vector3D(0.f, -1.f, 0.f),  gsl::Vector2D(0.5f,  0.333f)},

     // top
     Vertex{gsl::Vector3D(-1.f, 1.f,  1.f),    gsl::Vector3D(0.f, 1.f, 0.f),   gsl::Vector2D(0.25f, 0.666f)},
     Vertex{gsl::Vector3D( 1.f, 1.f,  1.f),    gsl::Vector3D(0.f, 1.f, 0.f),   gsl::Vector2D(0.5f,  0.666f)},
     Vertex{gsl::Vector3D(-1.f, 1.f, -1.f),    gsl::Vector3D(0.f, 1.f, 0.f),   gsl::Vector2D(0.25f, 0.999f)},
     Vertex{gsl::Vector3D( 1.f, 1.f, -1.f),    gsl::Vector3D(0.f, 1.f, 0.f),   gsl::Vector2D(0.5f,  0.999f)}
                      });

    temp.mIndices[0].insert( temp.mIndices[0].end(),
    { 0,  2,  1,  1,  2,  3,
      4,  6,  5,  5,  6,  7,
      8,  10,  9, 9,  10, 11,
      12, 14, 13, 13, 14, 15,
      16, 18, 17, 17, 18, 19,
      20, 22, 21, 21, 22, 23
                     });

    temp.mDrawType = GL_TRIANGLES;
    //temp.mTransform->mMatrix.scale(30.f);
    initMesh(temp, 0);

    return mMeshes.size()-1;    //returns index to last object

}


int MeshHandler::makeCube()
{
    //should check if this object is new before this!
    mMeshes.emplace_back(MeshData());
    MeshData &temp = mMeshes.back();
    temp.mVertices[0].push_back(Vertex(-0.5,-0.5,0.5   , 1, 0, 1));
    temp.mVertices[0].push_back(Vertex(0.5,-0.5,0.5    , 1, 0, 1));
    temp.mVertices[0].push_back(Vertex(0.5,0.5,0.5     , 1, 0, 1));

    temp.mVertices[0].push_back(Vertex(-0.5,-0.5,0.5   , 1, 0, 1));
    temp.mVertices[0].push_back(Vertex(0.5,0.5,0.5     , 1, 0, 1));
    temp.mVertices[0].push_back(Vertex(-0.5,0.5,0.5    , 1, 0, 1));

    temp.mVertices[0].push_back(Vertex(-0.5,-0.5,-0.5  , 1, 0, 0));
    temp.mVertices[0].push_back(Vertex(-0.5,-0.5,0.5   , 1, 0, 0));
    temp.mVertices[0].push_back(Vertex(-0.5,0.5,0.5    , 1, 0, 0));

    temp.mVertices[0].push_back(Vertex(-0.5,-0.5,-0.5  , 1, 0, 0));
    temp.mVertices[0].push_back(Vertex(-0.5,0.5,0.5    , 1, 0, 0));
    temp.mVertices[0].push_back(Vertex(-0.5,0.5,-0.5   , 1, 0, 0));

    temp.mVertices[0].push_back(Vertex(0.5,0.5,-0.5    , 0, 1, 0));
    temp.mVertices[0].push_back(Vertex(-0.5,0.5,-0.5   , 0, 1, 0));
    temp.mVertices[0].push_back(Vertex(-0.5,-0.5,-0.5  , 0, 1, 0));

    temp.mVertices[0].push_back(Vertex(0.5,0.5,-0.5    , 0, 1, 0));
    temp.mVertices[0].push_back(Vertex(-0.5,-0.5,-0.5  , 0, 1, 0));
    temp.mVertices[0].push_back(Vertex(0.5,-0.5,-0.5   , 0, 1, 0));

    temp.mVertices[0].push_back(Vertex(0.5,0.5,0.5     , 1, 0, 1));
    temp.mVertices[0].push_back(Vertex(0.5,0.5,-0.5    , 1, 0, 1));
    temp.mVertices[0].push_back(Vertex(0.5,-0.5,-0.5   , 1, 0, 1));

    temp.mVertices[0].push_back(Vertex(0.5,-0.5,0.5    , 1, 0, 1));
    temp.mVertices[0].push_back(Vertex(0.5,-0.5,-0.5   , 1, 0, 1));
    temp.mVertices[0].push_back(Vertex(0.5,0.5,0.5     , 1, 0, 1));

    temp.mVertices[0].push_back(Vertex(-0.5,-0.5,0.5   , 1, 1, 0));
    temp.mVertices[0].push_back(Vertex(-0.5,-0.5,-0.5  , 1, 1, 0));
    temp.mVertices[0].push_back(Vertex(0.5,-0.5,-0.5   , 1, 1, 0));

    temp.mVertices[0].push_back(Vertex(0.5,-0.5,0.5    , 1, 1, 0));
    temp.mVertices[0].push_back(Vertex(-0.5,-0.5,0.5   , 1, 1, 0));
    temp.mVertices[0].push_back(Vertex(0.5,-0.5,-0.5   , 1, 1, 0));

    temp.mVertices[0].push_back(Vertex(-0.5,0.5,0.5    , 1, 0, 0));
    temp.mVertices[0].push_back(Vertex(-0.5,0.5,-0.5   , 1, 0, 0));
    temp.mVertices[0].push_back(Vertex(0.5,0.5,-0.5    , 1, 0, 0));

    temp.mVertices[0].push_back(Vertex(0.5,0.5,0.5     , 1, 0, 0));
    temp.mVertices[0].push_back(Vertex(-0.5,0.5,0.5    , 1, 0, 0));
    temp.mVertices[0].push_back(Vertex(0.5,0.5,-0.5    , 1, 0, 0));
    temp.mDrawType = GL_TRIANGLES;

    //only LOD level 0
    initMesh(temp, 0);

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

    unsigned short width = heighMap.mColumns;   //Width - x-axis
    unsigned short depth = heighMap.mRows;      //Depth - z-axis

    float vertexXStart{0.f - width*horisontalSpacing/2};
    float vertexZStart{0.f + depth*horisontalSpacing/2};


    for(int d{0}; d < depth; ++d)
    {
        for(int w{0}; w < width; ++w)
        {

            float heightFromBitmap = heighMap.getHeightFromIndex(w + d*depth) * heightSpacing + heightPlacement;
            tempMesh.mVertices[0].emplace_back(gsl::Vector3D{vertexXStart + (w * horisontalSpacing), heightFromBitmap,
                                                             vertexZStart - (d * horisontalSpacing)}, normal, gsl::Vector2D{w / (width-1.f), d / (depth-1.f)});

            if(heightFromBitmap < tempMesh.mLowLeftBackCorner.y)
                tempMesh.mLowLeftBackCorner.y = heightFromBitmap;
            if(heightFromBitmap > tempMesh.mUpRightFrontCorner.y)
                tempMesh.mUpRightFrontCorner.y = heightFromBitmap;
        }
    }


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



    calculateHeighMapNormals(width, depth, tempMesh);
    tempMesh.mColliderRadius = 1.f;

    tempMesh.mDrawType = GL_TRIANGLES;
    initMesh(tempMesh, 0);

    return mMeshes.size()-1;
}

int MeshHandler::makeLAS(std::string fileName)
{
    mMeshes.emplace_back(MeshData());
    MeshData &temp = mMeshes.back();

    //qDebug() << "making lAas";
    ReadDatafromFile(fileName, temp);
    GenerateHeightMap(temp);

    return mMeshes.size()-1;
}

void MeshHandler::ReadDatafromFile(std::string fileName, MeshData &mesh)
{
    std::string line;


    std::string tempName{0};
    tempName = fileName;

    tempName = gsl::DataFilePath + tempName;

    qDebug() << "Reading " << tempName.c_str();
    std::ifstream fileIn;
    fileIn.open (tempName, std::ifstream::in);
    std::string x,y,z;
    if (fileIn.is_open())
    {
        qDebug() << "file read.";
        getline (fileIn,line);
        while ( getline (fileIn,line) )
        {
            //getting x positions
            x += line[0];
            x += line[1];
            x += line[2];
            x += line[3];
            x += line[4];
            x += line[5];
            x += line[6];
            x += line[7];
            x += line[8];

            //getting y positions
            y += line[10];
            y += line[11];
            y += line[12];
            y += line[13];
            y += line[14];
            y += line[15];
            y += line[16];
            y += line[17];
            y += line[18];
            y += line[19];
            //getting z positions
            z += line[21];
            z += line[22];
            z += line[23];
            z += line[24];
            z += line[25];
            z += line[26];

            //std::cout <<x<< " "<< y << " "<<z<< '\n';
            mesh.positions.push_back(Vertex(std::stof(x),std::stof(z),std::stof(y), (line[4] - '0')*0.1f ,(line[23]- '0')*0.1f ,(line[16]- '0')*0.1f ,0 ,0)); // setting data into Vertex
            x = "";
            y = "";
            z = "";

        }
        fileIn.close();
        //delete delta in the vertex, as it starts with insaneley big values :D
        RemoveDeltaPos(mesh);
        initMesh(mesh, 0);
    }
    else
    {
        QString string = QString::fromStdString( fileName);
        qDebug() << "Unable to open file: " << string;
    }

}



void MeshHandler::calculateHeighMapNormals(int width, int depth, MeshData &mesh)
{
    //TODO: This can be optimized and calculated while heightMap is read in

    //not tested for non square textures:
    if (width != depth)
    {
        //Logger::getInstance()->logText("Normals for ground not calclulated! Use square texture!");
        qDebug() <<  "normals for ground not calculated ";
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
    qDebug() << "Normals for ground calclulated";
}

void MeshHandler::RemoveDeltaPos(MeshData &mesh)
{


    float smallestX{0}, biggestX{0};
    float smallestY{0}, biggestY{0};
    float smallestZ{0}, biggestZ{0};

    //we take biggest delta between x,y and z, then we remove it from the vertexes to get a more appropreate number.
    for(auto i = 0; i<mesh .positions.size(); i++)
    {

        //gsml::Vector3d temp = positions[i].getVertex();
        float tempX = mesh .positions[i].get_xyz().getX();
        float tempY = mesh .positions[i].get_xyz().getY();
        float tempZ = mesh .positions[i].get_xyz().getZ();
        //qDebug() << "xyz "<< temp;
        //check size of x
        if(tempX < smallestX)
            smallestX = tempX;
        if(tempX > biggestX)
            biggestX = tempX;
        //check size of y
        if(tempY < smallestY)
            smallestY = tempY;
        if(tempY > biggestY)
            biggestY = tempY;
        //check size of Z
        if(tempZ < smallestZ)
            smallestZ = tempZ;
        if(tempZ > biggestZ)
            biggestZ = tempZ;
    }

    for(auto i = 0; i<mesh .positions.size(); i++)
    {

        gsl::Vector3D temp = mesh.positions[i].get_xyz();
        //qDebug() << "xyz "<< temp;
        //check size of x
        if(temp.getX() < smallestX)
            smallestX = temp.getX();
        if(temp.getX() > biggestX)
            biggestX = temp.getX();
        //check size of y
        if(temp.getY() < smallestY)
            smallestY = temp.getY();
        if(temp.getY() > biggestY)
            biggestY = temp.getY();
        //check size of Z
        if(temp.getZ() < smallestZ)
            smallestZ = temp.getZ();
        if(temp.getZ() > biggestZ)
            biggestZ = temp.getZ();
    }

    float deltaX{biggestX - smallestX}, deltaY{biggestY - smallestY }, deltaZ{biggestZ - smallestZ };
    qDebug() << "Delta x : "<<deltaX<< " Delta y: " << deltaY<< " Delta z: "<< deltaZ<< "\n";
    qDebug() << "Biggest x : "<< biggestX<< " smallest x: " << smallestX<< "\n";
    qDebug() << "Biggest y : "<<biggestY<< " smallest Y: " << smallestY<< "\n";
    qDebug() << "Biggest z : "<<biggestZ<< " smallest z: " << smallestZ<< "\n";


    for(auto i = 0; i<mesh .positions.size(); i++)
    {
        mesh .positions[i].set_xyz(deltaX - mesh .positions[i].get_xyz().getX(),
                             deltaY - mesh .positions[i].get_xyz().getY() ,
                             deltaZ - mesh .positions[i].get_xyz().getZ());
        //qDebug() << positions[i].getVertex();//.getX() << positions[i].getVertex().getY() << positions[i].getVertex().getZ() ;
    }

}

void MeshHandler::GenerateHeightMap(MeshData &mesh)
{


    float ofsetx = -100;
    float ofsetz = -100;
    float ofsety = -15;

    for(float x = 100; x<200; x+=1)
        for(float z =100; z<200; z+=1)
        {
            //get all height data :D
            float height1 = CalcHeight(    x,    z);
            float height2 = CalcHeight(  x+1,    z);
            float height3 = CalcHeight(    x,  z+1);
            float height4 = CalcHeight(    x,  z+1);
            float height5 = CalcHeight(  x+1,    z);
            float height6 = CalcHeight(  x+1,  z+1);

                                                                                        //use height date for colouring   //This order is like this because our
            mesh.mVertices[0].push_back(Vertex{ofsetx +  x, ofsety +height1,ofsetz +   z,       x/900, height1/100, z/1000,0,0}); //1
            mesh.mVertices[0].push_back(Vertex{ofsetx +x+1, ofsety +height2,ofsetz +   z,       x/900, height2/100, z/1000,0,0}); //2
            mesh.mVertices[0].push_back(Vertex{ofsetx +  x, ofsety +height3,ofsetz + z+1,       x/900, height3/100, z/1000,0,0}); //3
            mesh.mVertices[0].push_back(Vertex{ofsetx +  x, ofsety +height4,ofsetz + z+1,       x/900, height4/100, z/1000,0,0}); //4
            mesh.mVertices[0].push_back(Vertex{ofsetx +x+1, ofsety +height5,ofsetz +   z,       x/900, height5/100, z/1000,0,0}); //5
            mesh.mVertices[0].push_back(Vertex{ofsetx +x+1, ofsety +height6,ofsetz + z+1,       x/900, height6/100, z/1000,0,0}); //6
        }
}

float MeshHandler::CalcHeight(float x, float z)
{
    float height = 10.0f;


    int X = static_cast<int>(x);
    int Z = static_cast<int>(z);
    int counter =0;
    float collected = 0;
    height = PosArr[X][Z];

    if((X>15 && Z>15) /*&& height <= 2.0f */)
    {

        for(int i = -14; i < 14 ; i++)
        {
            for (int j = -14 ; j <14; j++ )
            {
                if(PosArr[X + i][Z + j] != 0)
                {
                    collected += PosArr[X + i][Z + j];
                    counter++;
                }
            }
        }
        height = ( collected)/(counter);
        //average height of surrounding area
    }
    else
    {
        height = PosArr[X][Z] ;
    }


    //do the average calc
    //qDebug() << height;
    //then return its
    return height;
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

std::vector<Vertex> MeshHandler::getPositions() const
{
    MeshData tempMesh;
    return tempMesh.positions;
}

std::vector<Vertex> MeshHandler::getmVertices() const
{
    MeshData tempMesh;
    return tempMesh.mVertices[0];
}

void MeshHandler::setPositions(const std::vector<Vertex> &value)
{
    MeshData tempMesh;
    tempMesh.positions = value;
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


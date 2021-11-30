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
        if (meshName.find("ball") != std::string::npos)
            meshIndex = makeBall(3);

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

int MeshHandler::makeBall(int n)
{
    m_rekursjoner = n;

    mMeshes.emplace_back(MeshData());
    MeshData &tempMesh = mMeshes.back();

    tempMesh.mVertices[0].reserve(3 * 8 * pow(4, m_rekursjoner));
    oktaederUnitBall(tempMesh);

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
    minMaxNormalize();
    //GenerateHeightMap(temp);


    int quadCoordX = 0;
    int quadCoordZ = 0;

    // Using min, max and step to find how many quads the TriangleSurface has in each direction
    const int quadAmountX = abs(xMax-xMin)/step; //qDebug() << quadAmountX;
    const int quadAmountZ = abs(zMax-zMin)/step; //qDebug() << quadAmountZ;
    //const int numOfQuads = quadAmountX*quadAmountZ; //qDebug() << numOfQuads;

    // An array of vectors that hold all the heights for each quad
    // Size of array is hardcoded, unsure how to do it from numOfQuads
    std::array<std::vector<float>,800> heights;
    //std::array<std::vector<float>,numOfQuads> heights;

    //An array that holds the average height of each quad
    float averageHeights[800];
    //float averageHeights[numOfQuads];



    //For every vertices
    //qDebug() << "Vertices size" << lasData.size();
    for(int i = 0; i < lasData.size(); i++)
    {
        //.. find what column the vertex is on
        for(int j = xMin; j < xMax; j+=step) // j = -5.0f, -3.0f, -1.0f, 1.0f, 3.0f, 5.0f
        {
            if(lasData[i].get_xyz().getX() > j && lasData[i].get_xyz().getX() < j + step)
            {
                quadCoordX = (j-xMin)/step;
            }
        }
        //.. find what row the vertex is on
        for(int j = zMin; j < zMax; j+=step)
        {
            if(lasData[i].get_xyz().getZ() > j && lasData[i].get_xyz().getZ() < j + step)
            {
                quadCoordZ = (j-zMin)/step;
            }
        }

        //Converts from row&column to vector-array index
        int vectorIndex = quadCoordZ*quadAmountZ + quadCoordX; //0-1199

        //Pushes the height(y) to the correct vector in the array
        heights[vectorIndex].push_back(lasData[i].get_xyz().getY());
    }

    //qDebug() << "size of height array " << heights.size();
    for(int i = 0; i < heights.size(); i++)
    {
        //qDebug() << "size of height vector " << i << heights[i].size();

        //Calculate the average of all heights in quad
        //.. and put it in the array of averageHeights

        //float sum = std::accumulate(heights[i].begin(), heights[i].end(), 0);

        float sum = 0;
        for(int j = 0; j < heights[i].size(); j++)
        {
            sum += heights[i][j];
        }
        sum = sum/heights[i].size();
        averageHeights[i] = sum;
    }

//    for(int i = 0; i < 25 ; i++)
//    {
//        qDebug() << "averageheight:" << i << " " << averageHeights[i];
//    }

    //Create triangulated surface
    float R = 10;
    float G = 50;
    float B = 75;
    for (float x = xMin; x < xMax-step; x+= step)
    {
        for(float z = zMin; z < zMax; z+= step)
        {
            int quadCoordX = (x-xMin)/step;
            int quadCoordZ = (z-zMin)/step;
            float u{(x + abs(xMin)) / (xMax + abs(xMin) + step)};
            float v{(z + abs(zMin)) / (zMax + abs(zMin) + step)};

            temp.mVertices[0].push_back(Vertex(x, averageHeights[quadCoordZ*quadAmountZ + quadCoordX], z,
                                                           R/255, averageHeights[quadCoordZ*quadAmountZ + quadCoordX]*G/255, B/255,u,v));

            temp.mVertices[0].push_back(Vertex(x, averageHeights[(quadCoordZ+1)*quadAmountZ + quadCoordX], z+step,
                                                           R/255, averageHeights[(quadCoordZ+1)*quadAmountZ + quadCoordX]*G/255, B/255, u, v+step));

            temp.mVertices[0].push_back(Vertex(x+step, averageHeights[quadCoordZ*quadAmountZ + quadCoordX+1], z,
                                                           R/255, averageHeights[quadCoordZ*quadAmountZ + quadCoordX+1]*G/255, B/255, u+step,v));



            temp.mVertices[0].push_back(Vertex(x+step, averageHeights[(quadCoordZ+1)*quadAmountZ + quadCoordX+1], z+step,
                                                           R/255, averageHeights[(quadCoordZ+1)*quadAmountZ + quadCoordX+1]*G/255, B/255, u+step, v+step));

            temp.mVertices[0].push_back(Vertex(x+step, averageHeights[quadCoordZ*quadAmountZ + quadCoordX+1], z,
                                                           R/255, averageHeights[quadCoordZ*quadAmountZ + quadCoordX+1]*G/255, B/255, u+step,v));

            temp.mVertices[0].push_back(Vertex(x, averageHeights[(quadCoordZ+1)*quadAmountZ + quadCoordX], z+step,
                                                           R/255, averageHeights[(quadCoordZ+1)*quadAmountZ + quadCoordX]*G/255, B/255, u, v+step));

        }
    }

    temp.mDrawType = GL_TRIANGLES;
    initMesh(temp,0);

    return mMeshes.size()-1;
}

void MeshHandler::ReadDatafromFile(std::string fileName, MeshData &mesh)
{
    /*
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
        //RemoveDeltaPos(mesh);
        //initMesh(mesh, 0);
    }
    else
    {
        QString string = QString::fromStdString( fileName);
        qDebug() << "Unable to open file: " << string;
    }
    */


    std::string tempName{0};
    tempName = fileName;

    tempName = gsl::DataFilePath + tempName;

    qDebug() << "Reading " << tempName.c_str();
    std::ifstream fileIn;
    fileIn.open (tempName, std::ifstream::in);

    if (!fileIn.is_open())
    {
        qDebug() << "Error, file couldn't be opened";
    }
    if (fileIn.is_open()) {
        int numberOfVertices;
        fileIn >> numberOfVertices;
        lasData.reserve(numberOfVertices);
        Vertex vertex{0,0,0};
        double tempX, tempY, tempZ;
        float x,y,z;
        for (int i=0; i < numberOfVertices; i++)
        {

             fileIn >> tempX >> tempZ >> tempY;
             //convert double to float since las data has a lot of decimals
             x = float(tempX);
             y = float(tempY);
             z = float(tempZ);

             if(i == 0)
             {
                 highestX = x;
                 highestY = y;
                 highestZ = z;
                 lowestX = x;
                 lowestY = y;
                 lowestZ = z;
             }

             if(tempX > highestX){ highestX = x;}
             if(tempY > highestY){ highestY = y;}
             if(tempZ > highestZ){ highestZ = z;}
             if(tempX < lowestX){ lowestX = x;}
             if(tempY < lowestY){ lowestY = y;}
             if(tempZ < lowestZ){ lowestZ = z;}

             vertex.set_xyz(x, y, z);
             lasData.push_back(vertex);
        }
        fileIn.close();
    }

    /*Print las data*/
    qDebug() << highestX << highestY << highestZ << lowestX << lowestY << lowestZ;
//    for(int i = 0; i < 1000; i++)
//    {
//        qDebug() << lasData[i].getXYZ().getX() << lasData[i].getXYZ().getY() << lasData[i].getXYZ().getZ() ;
//    }


}

void MeshHandler::minMaxNormalize()
{
    for(int i = 0; i < lasData.size(); i++)
    {
        float nX = xMin+(((lasData[i].get_xyz().getX() - lowestX)*(xMax-xMin)) / (highestX - lowestX));
        float nY = yMin+(((lasData[i].get_xyz().getY()  - lowestY)*(yMax-yMin)) / (highestY - lowestY));
        float nZ = zMin+(((lasData[i].get_xyz().getZ()  - lowestZ)*(zMax-zMin)) / (highestZ - lowestZ));

        lasData[i].set_xyz(nX,nY,nZ);
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

void MeshHandler::lagTriangel(const Vec3 &v1, const Vec3 &v2, const Vec3 &v3, MeshData &tempMesh )
{
    Vertex v{};

    v.set_xyz(v1.x, v1.y, v1.z);		// koordinater v.x = v1.x, v.y=v1.y, v.z=v1.z
    v.set_normal(v1.x, v1.y, v1.z);	// rgb
    v.set_st(0.0f, 0.0f);			// kan utelates
    tempMesh.mVertices[0].push_back(v);
    v.set_xyz(v2.x, v2.y, v2.z);
    v.set_normal(v2.x, v2.y, v2.z);
    v.set_st(1.0f, 0.0f);
    tempMesh.mVertices[0].push_back(v);
    v.set_xyz(v3.x, v3.y, v3.z);
    v.set_normal(v3.x, v3.y, v3.z);
    v.set_st(0.5f, 1.0f);
    tempMesh.mVertices[0].push_back(v);
}

void MeshHandler::subDivide(const Vec3 &a, const Vec3 &b, const Vec3 &c, int n, MeshData &tempMesh)
{
    if (n>0) {
        Vec3 v1 = a+b; v1.normalize();
        Vec3 v2 = a+c; v2.normalize();
        Vec3 v3 = c+b; v3.normalize();
        subDivide(a, v1, v2, n-1, tempMesh);
        subDivide(c, v2, v3, n-1, tempMesh);
        subDivide(b, v3, v1, n-1, tempMesh);
        subDivide(v3, v2, v1, n-1, tempMesh);
    } else {
        lagTriangel(a, b, c, tempMesh);
    }
}

void MeshHandler::oktaederUnitBall(MeshData &tempMesh)
{
    Vec3 v0{0, 0, 1};
    Vec3 v1{1, 0, 0};
    Vec3 v2{0, 1, 0};
    Vec3 v3{-1, 0, 0};
    Vec3 v4{0, -1, 0};
    Vec3 v5{0, 0, -1};

    subDivide(v0, v1, v2, m_rekursjoner, tempMesh);
    subDivide(v0, v2, v3, m_rekursjoner, tempMesh);
    subDivide(v0, v3, v4, m_rekursjoner, tempMesh);
    subDivide(v0, v4, v1, m_rekursjoner, tempMesh);
    subDivide(v5, v2, v1, m_rekursjoner, tempMesh);
    subDivide(v5, v3, v2, m_rekursjoner, tempMesh);
    subDivide(v5, v4, v3, m_rekursjoner, tempMesh);
    subDivide(v5, v1, v4, m_rekursjoner, tempMesh);
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


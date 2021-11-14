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

    //temp.mTransform->mMatrix.scale(30.f);
    initMesh(temp, 0);

    return mMeshes.size()-1;    //returns index to last object

}

void MeshHandler::updateParticles(const float dt)
{

    //qDebug() << "number of particles " << mMeshes.size();
    //MeshData &temp = mMeshes.back();
//    for ( uint i = 0 ; i < particles.size() ; ++i )
//    {
//        // subtract from the particles lifetime
//        particles[i].lifetime -= dt;

//        // if the lifetime is below 0 respawn the particle
//        if ( particles[i].lifetime <= 0.0f )
//        {
//            particles[i].position +=  0.05f;
//            particles[i].lifetime =  1.0f ;
//        }

//        // move the particle down depending on the delta time
//        particles[i].position -= gsl::Vector3D( 0.0f ,.0f ,  dt*2.0f );

//        // update the position buffer
//        positions[i*4+0] = particles[i].position[0];
//        positions[i*4+1] = particles[i].position[1];
//        positions[i*4+2] = particles[i].position[2];
//        positions[i*4+3] = particles[i].lifetime;
//        //qDebug() << "number of particles " << mMeshes.size();

//        //mMeshes[i].mTransform->mMatrix.setPosition(1.f);
//    }
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



//int MeshHandler::Heightmap(TextureHandler *texture, float horSpaceing, float verSpacing, float height)
//{
//    mTexture = texture;
//    mHorisontalSpacing = horSpaceing;
//    mHeightSpacing = verSpacing;
//    mHeightPlacement = height;

//    mWidth = mTexture->mColumns;
//    mDepth = mTexture->mRows;

//    //makeTerrain();

//    mMeshes.emplace_back(MeshData());
//    MeshData &temp = mMeshes.back();

//    gsl::Vector3D normal{0.f, 1.f, 0.f};


//    float totalWidthMeters = (mWidth-1) * mHorisontalSpacing;
//        vertexXStart = -totalWidthMeters / 2;
//        vertexZStart = totalWidthMeters / 2;

//            for(int d{0}; d < mDepth; ++d)
//            {
//                for(int w{0}; w < mWidth; ++w)
//                {
//                    float heightFromBitmap; //= mTexture->getHeightFromIndex(w + d*mDepth) * mHeightSpacing / 50.f + mHeightPlacement;
//                    temp.mVertices[0].emplace_back(gsl::Vector3D{vertexXStart + (w*mHorisontalSpacing), heightFromBitmap,
//                                                     vertexZStart - (d*mHorisontalSpacing)}, normal, gsl::Vector2D{w / (mWidth-1.f), d / (mDepth-1.f)});
//                }
//            }
//            // The grid is drawn in quads with diagonal from lower left to upper right
//            //          _ _
//            //         |/|/|
//            //          - -
//            //         |/|/|
//            //          - -
//            for(int d = 0; d < mDepth-1; ++d)        //mDepth - 1 because we draw the last quad from mDepth - 1 and in negative z direction
//            {
//                for(int w = 0; w < mWidth-1; ++w)    //mWidth - 1 because we draw the last quad from mWidth - 1 and in positive x direction
//                {

//                   temp.mIndices[0].emplace_back(w + d * mWidth);                  // 0 + 0 * mWidth               = 0
//                   temp.mIndices[0].emplace_back(w + d * mWidth + mWidth + 1);     // 0 + 0 * mWidth + mWidth + 1  = mWidth + 1
//                   temp.mIndices[0].emplace_back(w + d * mWidth + mWidth);         // 0 + 0 * mWidth + mWidth      = mWidth
//                   temp.mIndices[0].emplace_back(w + d * mWidth);                  // 0 + 0 * mWidth               = 0
//                   temp.mIndices[0].emplace_back(w + d * mWidth + 1);              // 0 + 0 * mWidth + 1           = 1
//                   temp.mIndices[0].emplace_back(w + d * mWidth + mWidth + 1);     // 0 + 0 * mWidth + mWidth + 1  = mWidth + 1
//                }
//            }
//            //temp.mTransform->mMatrix.identity();

//        /* calculate normals */


//    //not tested for non square textures:
//    if (mWidth != mDepth)
//    {
//        qDebug() << "Normals for ground not calclulated! Use square texture!";

//    }
//    unsigned short resolutionSquared= mWidth * mDepth;

//    //terrain has the diagonal of the quads pointing up and right, like |/| (not |\|)

//    //************************* Special case - bottom row has no vertices below **********************************
//    for(unsigned short i{0}; i < mWidth; i++)
//    {
//        //temporary normals for each of the 6 surrounding triangles
//        gsl::Vector3D surroundingNormals[6]{};

//        //the center vertex that we calculate the normal for
//        gsl::Vector3D center = temp.mVertices[0].at(i).get_xyz();

//        gsl::Vector3D first{};  //first vector will be crossed with
//        gsl::Vector3D second{}; //second vector - check right hand rule!

//        // a: first = north, second = west
//        //check if we are to the left, if so - skip:
//        //If first vertex - skip - already (0, 1, 0)
//        if(i != 0)
//        {
//            first = temp.mVertices[0].at(i+ mWidth).get_xyz() - center;
//            second = temp.mVertices[0].at(i-1).get_xyz() - center;
//            surroundingNormals[0] = first ^ second;
//        }
//        // b: first and second flips so we don't have to calculate a vector we already have!!
//        // second = north east, first = north
//        //check if we are at left, if so - skip
//        if( ((i + 1) % mWidth) == false)
//        {
//            second = temp.mVertices[0].at(i + mWidth + 1).get_xyz() - center;
//            //first = mVertices.at(i+ mWidth).get_xyz() - center;
//            surroundingNormals[1] = second ^ first;

//            //c: first = east, second = north east
//            first = temp.mVertices[0].at(i+1).get_xyz() - center;
//            //second = mVertices.at(i + mWidth + 1).get_xyz() - center;
//            surroundingNormals[2] = first ^ second;
//        }

//        //add all vectors and normalize
//        gsl::Vector3D result{};
//        for(int i{0}; i < 6; i++)
//        {
//            surroundingNormals[i].normalize();
//            result += surroundingNormals[i];
//        }
//        result.normalize();

//        //put the normal into the vertex
//        temp.mVertices[0].at(i).set_normal(result.x, result.y, result.z);
//    }

//    //calculate each of the triangles normal - omitting the outer vertices
//    //starting at mWidth + 1 == the second vertex on the second row
//    //ending each row at second to last vertex, to not hit the outer edge
//    //ending at resolutionSquared - mWidth - 2 == second to last vertex on the second to last row
//    for(unsigned short i = mWidth + 1 ; i < resolutionSquared - mWidth - 2; i++)
//    {
//        //if at the end of a row, jump to next
//        if( (i + 2) % mWidth == 0)
//        {   i += 2; //have to add 2 to get to the next correct index
//            continue;
//        }
//        //goes in a counter clockwise direction
//        //terrain has the diagonal of the quads pointing up and right, like |/| (not |\|)

//        //temporary normals for each of the 6 surrounding triangles
//        gsl::Vector3D surroundingNormals[6]{};

//        //the center vertex that we calculate the normal for
//        gsl::Vector3D center = temp.mVertices[0].at(i).get_xyz();

//        //first and second vector flips each time because second
//        //is the "first" at the next calculation.
//        //We don't want to calculate again!
//        gsl::Vector3D first{};
//        gsl::Vector3D second{};

//        // a 0: first = north, second = west
//        //check if we are to the left, if so - skip:
//        first = temp.mVertices[0].at(i+ mWidth).get_xyz() - center;
//        second = temp.mVertices[0].at(i-1).get_xyz() - center;
//        surroundingNormals[0] = first ^ second;

//        // b 1: first = south west, second = west
//        first = temp.mVertices[0].at(i - mWidth - 1).get_xyz() - center;
//        surroundingNormals[1] = second ^ first;

//        //c 2: first = south west, second = south
//        second = temp.mVertices[0].at(i - mWidth).get_xyz() - center;
//        surroundingNormals[2] = first ^ second;

//        //d 3: second = south, first = east
//        //check if we are to the left, if so - skip
//        first = temp.mVertices[0].at(i + 1).get_xyz() - center;
//        surroundingNormals[3] = second ^ first;

//        //e 4: first = east, second = north east
//        second = temp.mVertices[0].at(i + mWidth +1).get_xyz() - center;
//        surroundingNormals[4] = first ^ second;

//        //f 5: second = north east, first = north
//        first = temp.mVertices[0].at(i + mWidth).get_xyz() - center;
//        surroundingNormals[5] = second ^ first;

//        //add all vectors and normalize
//        gsl::Vector3D result{};
//        for(int i{0}; i < 6; i++)
//        {
//            surroundingNormals[i].normalize();
//            result += surroundingNormals[i];
//        }
//        result.normalize();

//        //put the normal into the vertex
//        temp.mVertices[0].at(i).set_normal(result.x, result.y, result.z);


//        /*   ---------------------------  */

//    }

//    temp.mDrawType = GL_TRIANGLES;
//    initMesh(temp, 0);

//    return mMeshes.size()-1;
//}

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

    float totalWidthMeters = (width - 1) * horisontalSpacing;


    calculateHeighMapNormals(width, depth, tempMesh);
    tempMesh.mColliderRadius = 1.f;


    initMesh(tempMesh, 0);

    return mMeshes.size()-1;
}


void MeshHandler::calculateHeighMapNormals(int width, int depth, MeshData &mesh)
{
    //TODO: This can be optimized and calculated while heightMap is read in

    //not tested for non square textures:
    if (width != depth)
    {
        //Logger::getInstance()->logText("Normals for ground not calclulated! Use square texture!");
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

MeshData MeshHandler::makeFrustum(Camera *camera)
{
    MeshData temp;

    gsl::Vector3D &cameraPos = camera->mPosition;
    gsl::Matrix4x4 &cameraRot = camera->mViewMatrix;

    float xMinus{cameraPos.x};
    float yMinus{cameraPos.y};
    float zMinus{cameraPos.z};



    temp.mVertices[0].insert( temp.mVertices[0].end(),
                {

       //Vertex data for back
       Vertex{xMinus, yMinus, zMinus,      1, 0.301, 0.933,          0.f, 0.f}    // 4


                      });


    temp.mDrawType = GL_LINES;
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


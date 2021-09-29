#include "resourcemanager.h"
#include <sstream>
#include <fstream>
#include <vector>
#include <QString>
#include <QDebug>

#include "systems/ecs/Components.h"
#include "systems/ecs/entity.h"
#include "vector3d.h"
#include "vector2d.h"
#include "vertex.h"
#include "constants.h"
resourceManager::resourceManager()
{
    mMeshComponents.reserve(gsl::MAX_MESHCOMPONENTS);
    mMeshHandler = new MeshHandler();
    //mTextureHandler = new TextureHandler();

}

resourceManager::~resourceManager()
{

}

resourceManager &resourceManager::getInstance()
{
    static resourceManager *mInstance = new resourceManager();
    return *mInstance;
}

//int resourceManager::makeAxis()
//{
//    //should check if this object is new before this!
//    mMeshComponents.emplace_back(MeshComponent());
//    MeshComponent &temp = mMeshComponents.back();

//    temp.mVertices.push_back(Vertex{0.f, 0.f, 0.f, 1.f, 0.f, 0.f});
//    temp.mVertices.push_back(Vertex{100.f, 0.f, 0.f, 1.f, 0.f, 0.f});
//    temp.mVertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 1.f, 0.f});
//    temp.mVertices.push_back(Vertex{0.f, 100.f, 0.f, 0.f, 1.f, 0.f});
//    temp.mVertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 0.f, 1.f});
//    temp.mVertices.push_back(Vertex{0.f, 0.f, 100.f, 0.f, 0.f, 1.f});

//    temp.mDrawType = GL_LINES;

//    initMesh(temp);

//    return mMeshComponents.size()-1;    //returns index to last object
//}

//int resourceManager::makeTriangle()
//{
//    //should check if this object is new before this!
//    mMeshComponents.emplace_back(MeshComponent());
//    MeshComponent &temp = mMeshComponents.back();
//    temp.mVertices.push_back(Vertex{-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  0.f, 0.f}); // Bottom Left
//    temp.mVertices.push_back(Vertex{0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.f}); // Bottom Right
//    temp.mVertices.push_back(Vertex{0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.f}); // Top

//    temp.mDrawType = GL_TRIANGLES;
//    initMesh(temp);

//    return mMeshComponents.size()-1;    //returns index to last object

//}

//int resourceManager::makeCube()
//{
//    mMeshComponents.emplace_back(MeshComponent());
//    MeshComponent &tempMesh = mMeshComponents.back();

//    tempMesh.mVertices.push_back(Vertex(-0.5,-0.5,0.5   , 1, 0, 1));
//    tempMesh.mVertices.push_back(Vertex(0.5,-0.5,0.5    , 1, 0, 1));
//    tempMesh.mVertices.push_back(Vertex(0.5,0.5,0.5     , 1, 0, 1));

//    tempMesh.mVertices.push_back(Vertex(-0.5,-0.5,0.5   , 1, 0, 1));
//    tempMesh.mVertices.push_back(Vertex(0.5,0.5,0.5     , 1, 0, 1));
//    tempMesh.mVertices.push_back(Vertex(-0.5,0.5,0.5    , 1, 0, 1));

//    tempMesh.mVertices.push_back(Vertex(-0.5,-0.5,-0.5  , 1, 0, 0));
//    tempMesh.mVertices.push_back(Vertex(-0.5,-0.5,0.5   , 1, 0, 0));
//    tempMesh.mVertices.push_back(Vertex(-0.5,0.5,0.5    , 1, 0, 0));

//    tempMesh.mVertices.push_back(Vertex(-0.5,-0.5,-0.5  , 1, 0, 0));
//    tempMesh.mVertices.push_back(Vertex(-0.5,0.5,0.5    , 1, 0, 0));
//    tempMesh.mVertices.push_back(Vertex(-0.5,0.5,-0.5   , 1, 0, 0));

//    tempMesh.mVertices.push_back(Vertex(0.5,0.5,-0.5    , 0, 1, 0));
//    tempMesh.mVertices.push_back(Vertex(-0.5,0.5,-0.5   , 0, 1, 0));
//    tempMesh.mVertices.push_back(Vertex(-0.5,-0.5,-0.5  , 0, 1, 0));

//    tempMesh.mVertices.push_back(Vertex(0.5,0.5,-0.5    , 0, 1, 0));
//    tempMesh.mVertices.push_back(Vertex(-0.5,-0.5,-0.5  , 0, 1, 0));
//    tempMesh.mVertices.push_back(Vertex(0.5,-0.5,-0.5   , 0, 1, 0));

//    tempMesh.mVertices.push_back(Vertex(0.5,0.5,0.5     , 1, 0, 1));
//    tempMesh.mVertices.push_back(Vertex(0.5,0.5,-0.5    , 1, 0, 1));
//    tempMesh.mVertices.push_back(Vertex(0.5,-0.5,-0.5   , 1, 0, 1));

//    tempMesh.mVertices.push_back(Vertex(0.5,-0.5,0.5    , 1, 0, 1));
//    tempMesh.mVertices.push_back(Vertex(0.5,-0.5,-0.5   , 1, 0, 1));
//    tempMesh.mVertices.push_back(Vertex(0.5,0.5,0.5     , 1, 0, 1));

//    tempMesh.mVertices.push_back(Vertex(-0.5,-0.5,0.5   , 1, 1, 0));
//    tempMesh.mVertices.push_back(Vertex(-0.5,-0.5,-0.5  , 1, 1, 0));
//    tempMesh.mVertices.push_back(Vertex(0.5,-0.5,-0.5   , 1, 1, 0));

//    tempMesh.mVertices.push_back(Vertex(0.5,-0.5,0.5    , 1, 1, 0));
//    tempMesh.mVertices.push_back(Vertex(-0.5,-0.5,0.5   , 1, 1, 0));
//    tempMesh.mVertices.push_back(Vertex(0.5,-0.5,-0.5   , 1, 1, 0));

//    tempMesh.mVertices.push_back(Vertex(-0.5,0.5,0.5    , 1, 0, 0));
//    tempMesh.mVertices.push_back(Vertex(-0.5,0.5,-0.5   , 1, 0, 0));
//    tempMesh.mVertices.push_back(Vertex(0.5,0.5,-0.5    , 1, 0, 0));

//    tempMesh.mVertices.push_back(Vertex(0.5,0.5,0.5     , 1, 0, 0));
//    tempMesh.mVertices.push_back(Vertex(-0.5,0.5,0.5    , 1, 0, 0));
//    tempMesh.mVertices.push_back(Vertex(0.5,0.5,-0.5    , 1, 0, 0));

//    tempMesh.mDrawType = GL_TRIANGLES;
//    initMesh(tempMesh);

//    return mMeshComponents.size()-1;

//}

void resourceManager::makeOBB(gsl::Vector3D &vertexIn)
{
    //testing min
    if(vertexIn.x < mMinCenter.x)
        mMinCenter.x = vertexIn.x;
    if(vertexIn.y < mMinCenter.y)
        mMinCenter.y = vertexIn.y;
    if(vertexIn.z < mMinCenter.z)
        mMinCenter.z = vertexIn.z;

    //testing max
    if(vertexIn.x > mMaxRadius.x)
        mMaxRadius.x = vertexIn.x;
    if(vertexIn.y > mMaxRadius.y)
        mMaxRadius.y = vertexIn.y;
    if(vertexIn.z > mMaxRadius.z)
        mMaxRadius.z = vertexIn.z;

}

bool resourceManager::checkCollideRM(gsl::Vector3D Pmin, gsl::Vector3D Pmax)
{
    bool result;

//    qDebug() << BoundingBoxMax;
//    qDebug() << BoundingBoxMin;

    if(BoundingBoxMax.x >= Pmin.x &&
       BoundingBoxMin.x <= Pmax.x &&
       BoundingBoxMax.y >= Pmin.y &&
       BoundingBoxMin.y <= Pmax.y &&
       BoundingBoxMax.z <= Pmin.z &&
       BoundingBoxMin.z >= Pmax.z)
    {
        result = true;
    }

    else
    {
        result = false;
    }

//    qDebug() << result;
    return result;
}

void resourceManager::setBoundingBoxRM(gsl::Vector3D Min, gsl::Vector3D Max)
{
    BoundingBoxMin = Min;
    BoundingBoxMax = Max;
}

MeshData resourceManager::makeLineBox(std::string meshName)
{
    return mMeshHandler->makeLineBox(meshName);
}

MeshData resourceManager::makeCircleSphere(float radius, bool rgbColor)
{
    return mMeshHandler->makeCircleSphere(radius, rgbColor);
}
Entity* resourceManager::makeEntity(std::string meshName)
{

    int meshIndex = mMeshHandler->makeMesh(meshName);

    Entity* tempObject = new Entity();

    MeshComponent* tempMesh = new MeshComponent();

   // tempMesh->mVAO = mMeshHandler->mMeshes.at(meshIndex).mVAO;
    //tempMesh->mVertexCount = mMeshHandler->mMeshes.at(meshIndex).mVertexCount;
  //  tempMesh->mIndexCount = mMeshHandler->mMeshes.at(meshIndex).mIndexCount;
   //Dangerous, because vector can resize and will move pointers:


   // tempMesh->mDrawType = mMeshHandler->mMeshes.at(meshIndex).mDrawType;
  //  tempMesh->mColliderRadius = mMeshHandler->mMeshes.at(meshIndex).mColliderRadius;
  //  tempObject->mMesh = tempMesh;

  //  tempObject->mMaterial = new MaterialComponent();
  //  tempObject->mTransform = new TransformComponent();

    return tempObject;

//return 0;

}

/*
void resourceManager::initMesh(MeshComponent &tempMeshComp)
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &tempMeshComp.mVAO );
    glBindVertexArray( tempMeshComp.mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &tempMeshComp.mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, tempMeshComp.mVBO );

    glBufferData( GL_ARRAY_BUFFER, tempMeshComp.mVertices.size()*sizeof(Vertex), tempMeshComp.mVertices.data(), GL_STATIC_DRAW );

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
    if(tempMeshComp.mIndices.size() > 0) {
        glGenBuffers(1, &tempMeshComp.mEAB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tempMeshComp.mEAB);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, tempMeshComp.mIndices.size() * sizeof(GLuint), tempMeshComp.mIndices.data(), GL_STATIC_DRAW);
    }

    glBindVertexArray(0);

    tempMeshComp.mIndexCount= tempMeshComp.mIndices.size();
    tempMeshComp.mVertexCount = tempMeshComp.mVertices.size();
}
*/
//int resourceManager::readObj(std::string filename)
//{
//    //should check if this object is new before this!
//       mMeshComponents.emplace_back(MeshComponent());
//       MeshComponent &temp = mMeshComponents.back();

//       std::ifstream fileIn;
//       fileIn.open (filename, std::ifstream::in);
//       if(!fileIn)
//       {
//           qDebug() << "ERROR: Could not open file for reading: " << QString::fromStdString(filename);
//           qDebug() << "****** using arbitrary mesh as replacement!";
//           return 0;    //hack - this will crash if no meshes are made yet
//       }
//       //One line at a time-variable
//       std::string oneLine;
//       //One word at a time-variable
//       std::string oneWord;

//       std::vector<gsl::Vector3D> tempVertecies;
//       std::vector<gsl::Vector3D> tempNormals;
//       std::vector<gsl::Vector2D> tempUVs;

//       //    std::vector<Vertex> mVertices;    //made in VisualObject
//       //    std::vector<GLushort> mIndices;   //made in VisualObject

//       // Varible for constructing the indices vector
//       unsigned int temp_index = 0;

//       //Reading one line at a time from file to oneLine
//       while(std::getline(fileIn, oneLine))
//       {
//           //Doing a trick to get one word at a time
//           std::stringstream sStream;
//           //Pushing line into stream
//           sStream << oneLine;
//           //Streaming one word out of line
//           oneWord = ""; //resetting the value or else the last value might survive!
//           sStream >> oneWord;

//           if (oneWord == "#")
//           {
//               //Ignore this line
//               //            qDebug() << "Line is comment "  << QString::fromStdString(oneWord);
//               continue;
//           }
//           if (oneWord == "")
//           {
//               //Ignore this line
//               //            qDebug() << "Line is blank ";
//               continue;
//           }
//           if (oneWord == "v")
//           {
//               //            qDebug() << "Line is vertex "  << QString::fromStdString(oneWord) << " ";
//               gsl::Vector3D tempVertex;
//               sStream >> oneWord;
//               tempVertex.x = std::stof(oneWord);
//               sStream >> oneWord;
//               tempVertex.y = std::stof(oneWord);
//               sStream >> oneWord;
//               tempVertex.z = std::stof(oneWord);

//               //Vertex made - pushing it into vertex-vector
//               tempVertecies.push_back(tempVertex);

//               continue;
//           }
//           if (oneWord == "vt")
//           {
//               //            qDebug() << "Line is UV-coordinate "  << QString::fromStdString(oneWord) << " ";
//               gsl::Vector2D tempUV;
//               sStream >> oneWord;
//               tempUV.x = std::stof(oneWord);
//               sStream >> oneWord;
//               tempUV.y = std::stof(oneWord);

//               //UV made - pushing it into UV-vector
//               tempUVs.push_back(tempUV);

//               continue;
//           }
//           if (oneWord == "vn")
//           {
//               //            qDebug() << "Line is normal "  << QString::fromStdString(oneWord) << " ";
//               gsl::Vector3D tempNormal;
//               sStream >> oneWord;
//               tempNormal.x = std::stof(oneWord);
//               sStream >> oneWord;
//               tempNormal.y = std::stof(oneWord);
//               sStream >> oneWord;
//               tempNormal.z = std::stof(oneWord);

//               //Vertex made - pushing it into vertex-vector
//               tempNormals.push_back(tempNormal);
//               continue;
//           }
//           if (oneWord == "f")
//           {
//               //            qDebug() << "Line is a face "  << QString::fromStdString(oneWord) << " ";
//               //int slash; //used to get the / from the v/t/n - format
//               int index, normal, uv;
//               for(int i = 0; i < 3; i++)
//               {
//                   sStream >> oneWord;     //one word read
//                   std::stringstream tempWord(oneWord);    //to use getline on this one word
//                   std::string segment;    //the numbers in the f-line
//                   std::vector<std::string> segmentArray;  //temp array of the numbers
//                   while(std::getline(tempWord, segment, '/')) //splitting word in segments
//                   {
//                       segmentArray.push_back(segment);
//                   }
//                   index = std::stoi(segmentArray[0]);     //first is vertex
//                   if (segmentArray[1] != "")              //second is uv
//                       uv = std::stoi(segmentArray[1]);
//                   else
//                   {
//                       //qDebug() << "No uvs in mesh";       //uv not present
//                       uv = 0;                             //this will become -1 in a couple of lines
//                   }
//                   normal = std::stoi(segmentArray[2]);    //third is normal

//                   //Fixing the indexes
//                   //because obj f-lines starts with 1, not 0
//                   --index;
//                   --uv;
//                   --normal;

//                   if (uv > -1)    //uv present!
//                   {
//                       Vertex tempVert(tempVertecies[index], tempNormals[normal], tempUVs[uv]);
//                       temp.mVertices.push_back(tempVert);
//                   }
//                   else            //no uv in mesh data, use 0, 0 as uv
//                   {
//                       Vertex tempVert(tempVertecies[index], tempNormals[normal], gsl::Vector2D(0.0f, 0.0f));
//                       temp.mVertices.push_back(tempVert);
//                   }
//                   temp.mIndices.push_back(temp_index++);
//               }
//               continue;
//           }
//       }

//       //beeing a nice boy and closing the file after use
//       fileIn.close();

//       initMesh(temp);

//       qDebug() << QString::fromStdString(filename) << "successfully loaded";

//       return mMeshComponents.size()-1;    //returns index to last object
//}




//void resourceManager::setOBBCollider(MAX_ENTITIES_TYPE entityID, gsl::Vector3D min, gsl::Vector3D max)
//{

//     mRenderwindow->mEntity[entityID]->mCollider->mType = gsl::OBB;
//     mRenderwindow->mEntity[entityID]->mCollider->mMinCenter = min;
//     mRenderwindow->mEntity[entityID]->mCollider->mMaxRadius = max;
//     mRenderwindow->mEntity[entityID]->isCollidable = true;

//}

//bool resourceManager::testCollision(Entity *a, Entity *b)
//{
//    if (a->mCollider->mType == gsl::OBB && b->mCollider->mType == gsl::OBB)
//        {
//            return a->mCollider->mMaxRadius >= b->mCollider->mMinCenter
//                    && b->mCollider->mMaxRadius >= a->mCollider->mMinCenter;
//        }

//    else return false;
// //   return false;
//}
//void resourceManager::updateColliders()
//{
//    for (MAX_ENTITIES_TYPE i{0}; i < mRenderwindow->mEntity.size(); i++)
//    {
//        if (mRenderwindow->mEntity[i]->isCollidable)
//        {
//             if (mRenderwindow->mEntity[i]->mCollider->mType == gsl::OBB)
//                 setOBBCollider(i, mRenderwindow->mEntity[i]->mTransform->position
//                  - mRenderwindow->mEntity[i]->mTransform->scale,
//                    mRenderwindow->mEntity[i]->mTransform->position
//                  + mRenderwindow->mEntity[i]->mTransform->scale
//                 );
//             //else if (mRenderwindow->mEntity[i]->mCollider->mType == gsl::SPHERE) setSphereCollider(i, 0, mRenderwindow->mEntity[i].mCollider.bIsDynamic);

//        }
//    }
//}

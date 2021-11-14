#include "meshhandler.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <qvector3d.h>
#include <qvector2d.h>
#include <vertex.h>

#include "components.h"

//For ObjMesh bruker vi Ole sin kode, og QVector3D/2D istedenfor egen vector klasse. Jeg ser ingen grunn til å lage en egen obj import klasse,
//da denne er helt fin.
// Forskjellen ligger i at vår visualobject klassse er såppass annerledes at denne nå er standalone.

MeshHandler::MeshHandler()
{

}

MeshHandler::~MeshHandler()
{

}

void MeshHandler::init(MeshComponent &MeshComp, int LODlvl)
{
        //must call this to use OpenGL functions
        initializeOpenGLFunctions();

        //Vertex Array Object - VAO
        glGenVertexArrays( 1, &MeshComp.mVAO[LODlvl] );
        glBindVertexArray( MeshComp.mVAO[LODlvl] );

        //Vertex Buffer Object to hold vertices - VBO
        glGenBuffers( 1, &MeshComp.mVBO[LODlvl] );
        glBindBuffer( GL_ARRAY_BUFFER, MeshComp.mVBO[LODlvl] );

        glBufferData( GL_ARRAY_BUFFER, MeshComp.mVertices[LODlvl].size()*sizeof(Vertex), MeshComp.mVertices[LODlvl].data(), GL_STATIC_DRAW );

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
        if(MeshComp.mIndices[LODlvl].size() > 0) {
            glGenBuffers(1, &MeshComp.mEAB[LODlvl]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MeshComp.mEAB[LODlvl]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, MeshComp.mIndices[LODlvl].size() * sizeof(GLuint), MeshComp.mIndices[LODlvl].data(), GL_STATIC_DRAW);
        }

        glBindVertexArray(0);
}

void MeshHandler::readFile(std::string filename, MeshComponent *MeshComp, int LODlvl, CollisionComponent *CollComp, MeshComponent *CollLines)
{
    std::string tempFileName{0};
    if(LODlvl == 0)
    {
        tempFileName = filename;
    }else
    {
        filename.erase(filename.find(".obj"));
        tempFileName = filename + "_" + std::to_string(LODlvl);
        tempFileName = tempFileName + ".obj";
    }
    //qDebug() << "HERE filename: " << QString::fromStdString(tempFileName);
    std::ifstream fileIn;
    fileIn.open (tempFileName, std::ifstream::in);
    if(!fileIn)
        qDebug() << "Could not open file for reading: " << QString::fromStdString(tempFileName);

    //One line at a time-variable
    std::string oneLine;
    //One word at a time-variable
    std::string oneWord;

    std::vector<QVector3D> tempVertecies;
    std::vector<QVector3D> tempNormals;
    std::vector<QVector2D> tempUVs;

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
            QVector3D tempVertex;
            sStream >> oneWord;
            tempVertex.setX(std::stof(oneWord));
            sStream >> oneWord;
            tempVertex.setY(std::stof(oneWord));
            sStream >> oneWord;
            tempVertex.setZ(std::stof(oneWord));

            //Vertex made - pushing it into vertex-vector
            tempVertecies.push_back(tempVertex);


            // For making the collision box
            if(LODlvl == 0)
                findCollisionCorners(CollComp, tempVertex);

            continue;
        }
        if (oneWord == "vt")
        {
            //            qDebug() << "Line is UV-coordinate "  << QString::fromStdString(oneWord) << " ";
            QVector2D tempUV;
            sStream >> oneWord;
            tempUV.setX(std::stof(oneWord));
            sStream >> oneWord;
            tempUV.setY(std::stof(oneWord));

            //UV made - pushing it into UV-vector
            tempUVs.push_back(tempUV);

            continue;
        }
        if (oneWord == "vn")
        {
            //            qDebug() << "Line is normal "  << QString::fromStdString(oneWord) << " ";
            QVector3D tempNormal;
            sStream >> oneWord;
            tempNormal.setX(std::stof(oneWord));
            sStream >> oneWord;
            tempNormal.setY(std::stof(oneWord));
            sStream >> oneWord;
            tempNormal.setZ(std::stof(oneWord));

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
                    MeshComp->mVertices[LODlvl].push_back(tempVert);
                }
                else            //no uv in mesh data, use 0, 0 as uv
                {
                    Vertex tempVert(tempVertecies[index], tempNormals[normal], QVector2D(0.0f, 0.0f));
                    MeshComp->mVertices[LODlvl].push_back(tempVert);
                }
                MeshComp->mIndices[LODlvl].push_back(temp_index++);
            }
            continue;
        }
    }
    //beeing a nice boy and closing the file after use
    fileIn.close();

    if(LODlvl == 0)
        makeCollisionBox(CollComp,CollLines);

    init(*MeshComp, LODlvl);
}

void MeshHandler::createXYZAxis(MeshComponent *MeshComp, CollisionComponent *CollisionComponent, MeshComponent *CollLines)
{
    MeshComp->mVertices[0].push_back(Vertex{0,0,0,1,0,0});
    MeshComp->mVertices[0].push_back(Vertex{3,0,0,1,0,0});
    MeshComp->mVertices[0].push_back(Vertex{0,0,0,0,1,0});
    MeshComp->mVertices[0].push_back(Vertex{0,3,0,0,1,0});
    MeshComp->mVertices[0].push_back(Vertex{0,0,0,0,0,1});
    MeshComp->mVertices[0].push_back(Vertex{0,0,3,0,0,1});
    MeshComp->mDrawType = GL_LINES;

    init(*MeshComp, 0);
    //Min
        CollisionComponent->minCorner.setX(0.f);
        CollisionComponent->minCorner.setY(0.f);
        CollisionComponent->minCorner.setZ(0.f);

        CollisionComponent->maxCorner.setX(0.f);
        CollisionComponent->maxCorner.setY(0.f);
        CollisionComponent->maxCorner.setZ(0.f);

        CollisionComponent->mRaidus = 0.f;


        CollLines->mVertices->push_back(Vertex(0,0,0, 0,0,0,  0,0));
        CollLines->mVertices->push_back(Vertex(0,0,0, 0,0,0,  0,0));
        CollLines->mVertices->push_back(Vertex(0,0,0, 0,0,0,  0,0));
        CollLines->mVertices->push_back(Vertex(0,0,0, 0,0,0,  0,0));

        CollLines->mVertices->push_back(Vertex(0,0,0, 0,0,0,  0,0));
        CollLines->mVertices->push_back(Vertex(0,0,0, 0,0,0,  0,0));
        CollLines->mVertices->push_back(Vertex(0,0,0, 0,0,0,  0,0));
        CollLines->mVertices->push_back(Vertex(0,0,0, 0,0,0,  0,0));

//        CollLines->mIndices[0].insert( CollLines->mIndices[0].end(),
//        { 0, 1, 1, 2, 2, 3, 3, 0,
//          4, 5, 5, 6, 6, 7, 7, 4,
//          0, 4, 3, 7,
//          1, 5, 2, 6
//                         });

        CollLines->mDrawType = GL_LINES;

        init(*CollLines,0);
}

void MeshHandler::createCreateTerrain(std::string filename, MeshComponent *MeshComp, CollisionComponent *CollisionComponent, MeshComponent *CollLines)
{

    std::vector<gsl::Vector3D> mAllDataPoints;

    float vx,vy,vz;
    float xMin;
    float xMax;
    float zMin;
    float zMax;
    float yMin;
    float yMax;
    unsigned int n;


    std::ifstream inn;
    inn.open(filename.c_str());

    if(inn.is_open())
    {
        gsl::Vector3D pos;
        inn>>n;
        int loopLenth = n;
//        float i{0};
        mAllDataPoints.reserve(n);



        for(int i{0}; i<loopLenth;i++)
        {
            inn >> vx;
            inn >> vz;
            inn >> vy;
            float x = float(vx);
            float z = float(vz);
            float y = float(vy);

            pos.setX(x);
            pos.setZ(z);
            pos.setY(y);

            mAllDataPoints.push_back(pos);


            static bool once = true;
            if(once)
            {
                xMin = x;
                xMax = x;
                zMin = z;
                zMax = z;
                yMin = y;
                yMax = y;
                once = false;
            }
            if(x < xMin)
                xMin = x;

            if(x > xMax)
                xMax = x;

            if(z < zMin)
                zMin = z;

            if(z > zMax)
                zMax = z;

            if(y < yMin)
                yMin = y;

            if(y > yMax)
                yMax = y;

        }
    inn.close();

        for(unsigned long long i = 0; i < loopLenth;i++)
        {
            mAllDataPoints[i].setX(mAllDataPoints[i].getX() - xMin);
            mAllDataPoints[i].setZ(mAllDataPoints[i].getZ() - zMin);
            mAllDataPoints[i].setY(mAllDataPoints[i].getY() - yMin);
        }
    }
    xMax = xMax-xMin;
    xMin = xMin-xMin;
    zMax = zMax-zMin;
    zMin = zMin-zMin;
    yMax = yMax-yMin;
    yMin = yMin-yMin;

    CollisionComponent->minCorner.setX(xMin);
    CollisionComponent->minCorner.setY(yMin);
    CollisionComponent->minCorner.setZ(zMin);

    CollisionComponent->maxCorner.setX(xMax);
    CollisionComponent->maxCorner.setY(yMax);
    CollisionComponent->maxCorner.setZ(zMax);

    // is only used for frustum culling so doesnt have to accurate.
    CollisionComponent->mRaidus = xMax*2;

    // 998 er hardcodet siden nå er gridsize akkuratt 1 med den dataen jeg bruker nå;
    float gridSizeX = xMax / 998;
    qDebug() << "GridsizeX" << gridSizeX;
    // 1457 er hardcodet siden nå er gridsize akkuratt 1 med den dataen jeg bruker nå;
    float gridSizeZ = zMax / 1457;
    qDebug() << "GridsizeY" << gridSizeZ;


    CollLines->mVertices[0].push_back(Vertex(xMin, yMin, zMax,   1,0,0,  0,0));
    CollLines->mVertices[0].push_back(Vertex(xMax, yMin, zMax,   1,0,0,  0,0));
    CollLines->mVertices[0].push_back(Vertex(xMax, yMax, zMax,   1,0,0,  0,0));
    CollLines->mVertices[0].push_back(Vertex(xMin, yMax, zMax,   1,0,0,  0,0));

    CollLines->mVertices[0].push_back(Vertex(xMin, yMin, zMin,   1,0,0,  0,0));
    CollLines->mVertices[0].push_back(Vertex(xMax, yMin, zMin,   1,0,0,  0,0));
    CollLines->mVertices[0].push_back(Vertex(xMax, yMax, zMin,   1,0,0,  0,0));
    CollLines->mVertices[0].push_back(Vertex(xMin, yMax, zMin,   1,0,0,  0,0));

    CollLines->mIndices[0].insert( CollLines->mIndices[0].end(),
    { 0, 1, 1, 2, 2, 3, 3, 0,
      4, 5, 5, 6, 6, 7, 7, 4,
      0, 4, 3, 7,
      1, 5, 2, 6 });

    CollLines->mDrawType = GL_LINES;
    init(*CollLines,0);
    float gridSize {50};

//    heights2D.reserve((xMax*zMax/gridSize));

    for(int z = int(zMin); z < zMax;z+=gridSize)
    {
        for(int x = int(xMax);x > xMin; x-=gridSize)
        {
            for(auto it : mAllDataPoints)
            {
                if(it.x > x-gridSize && it.x < x && it.z > z && it.z < z + gridSize)
                {
                    //heights2D.push_back(gsl::Vector3D(x,it.y,z));
                    MeshComp->mVertices[0].push_back(Vertex(x,it.y,z, 1,0,0,  0,0));
//                    it=0;
                    goto jump;
                }
            }
            if(!MeshComp->mVertices->empty())
            {
                MeshComp->mVertices->push_back(MeshComp->mVertices->at(MeshComp->mVertices->size()-1));
                continue;
            }else
            {
                MeshComp->mVertices[0].push_back(Vertex(x,0,z, 1,0,0,  0,0));
                continue;
            }
            jump:;
        }
    }

    // Code from 3DProg just  to test, neet to set the correct indecies
    // should problably use a 2d array to have more control of each vertex;
    int c;
    for(GLuint i{0}; i < MeshComp->mVertices[0].size() - (xMax/gridSize) -1; i++)
    {
        if(c == (xMax/gridSize) -1)
        {
            c=0;
            continue;
        }
        MeshComp->mIndices->push_back(i);
        MeshComp->mIndices->push_back(i+(xMax/gridSize));
        MeshComp->mIndices->push_back(i+gridSize);

        MeshComp->mIndices->push_back(i+(xMax/gridSize));
        MeshComp->mIndices->push_back(i+(xMax/gridSize)+gridSize);
        MeshComp->mIndices->push_back(i+gridSize);
        c++;
    }


init(*MeshComp,0);



//        qDebug() << "xmin: " << xMin << "xmax: " << xMax <<  "ymin: " << yMin << "ymax: " << yMax << "zmin: " << zMin << "zmax: " << zMax;





//    for(int xgrid = 0; xgrid < 1000; xgrid++)
//    {
//        for(int zgrid = 0; x)
//    }






}

void MeshHandler::findCollisionCorners(CollisionComponent *CollComp, QVector3D &vertex)
{
        //Min
        if(vertex.x() < CollComp->minCorner.x())
        {
            CollComp->minCorner.setX(vertex.x());
        }
        if(vertex.y() < CollComp->minCorner.y())
        {
            CollComp->minCorner.setY(vertex.y());
        }
        if(vertex.z() < CollComp->minCorner.z())
        {
            CollComp->minCorner.setZ(vertex.z());
        }

        //Max
        if(vertex.x() > CollComp->maxCorner.x())
        {
            CollComp->maxCorner.setX(vertex.x());
        }
        if(vertex.y() > CollComp->maxCorner.y())
        {
            CollComp->maxCorner.setY(vertex.y());
        }
        if(vertex.z() > CollComp->maxCorner.z())
        {
            CollComp->maxCorner.setZ(vertex.z());
        }

        if(vertex.length() > CollComp->mRaidus)
        {
            CollComp->mRaidus = vertex.length();
        }

}


void MeshHandler::makeCollisionBox(CollisionComponent* CollisionComp, MeshComponent* CollisionLines)
{
    float minX = CollisionComp->minCorner.x();
    float minY = CollisionComp->minCorner.y();
    float minZ = CollisionComp->minCorner.z();
    float maxX = CollisionComp->maxCorner.x();
    float maxY = CollisionComp->maxCorner.y();
    float maxZ = CollisionComp->maxCorner.z();

    CollisionLines->mVertices->push_back(Vertex(minX, minY, maxZ,   1,0,0,  0,0));
    CollisionLines->mVertices->push_back(Vertex(maxX, minY, maxZ,   1,0,0,  0,0));
    CollisionLines->mVertices->push_back(Vertex(maxX, maxY, maxZ,   1,0,0,  0,0));
    CollisionLines->mVertices->push_back(Vertex(minX, maxY, maxZ,   1,0,0,  0,0));

    CollisionLines->mVertices->push_back(Vertex(minX, minY, minZ,   1,0,0,  0,0));
    CollisionLines->mVertices->push_back(Vertex(maxX, minY, minZ,   1,0,0,  0,0));
    CollisionLines->mVertices->push_back(Vertex(maxX, maxY, minZ,   1,0,0,  0,0));
    CollisionLines->mVertices->push_back(Vertex(minX, maxY, minZ,   1,0,0,  0,0));

//    CollisionLines->mVertices[0].insert( CollisionLines->mVertices[0].end(),
//      {//Vertex data for front points       color                       uv
//       Vertex{minX, minY, maxZ,       1, 0.301, 0.933,          0.f, 0.f},     // 0
//       Vertex{maxX, minY, maxZ,       1, 0.301, 0.933,          0.f, 0.f},
//       Vertex{maxX, maxY, maxZ,       1, 0.301, 0.933,          0.f, 0.f},
//       Vertex{minX, maxY, maxZ,       1, 0.301, 0.933,          0.f, 0.f},
//       //Vertex data for back
//       Vertex{minX, minY, minZ,      1, 0.301, 0.933,          0.f, 0.f},    // 4
//       Vertex{maxX, minY, minZ,      1, 0.301, 0.933,          0.f, 0.f},
//       Vertex{maxX, maxY, minZ,      1, 0.301, 0.933,          0.f, 0.f},
//       Vertex{minX, maxY, minZ,      1, 0.301, 0.933,          0.f, 0.f},
//                      });

    CollisionLines->mIndices[0].insert( CollisionLines->mIndices[0].end(),
    { 0, 1, 1, 2, 2, 3, 3, 0,
      4, 5, 5, 6, 6, 7, 7, 4,
      0, 4, 3, 7,
      1, 5, 2, 6
                     });

    CollisionLines->mDrawType = GL_LINES;

    init(*CollisionLines,0);
}

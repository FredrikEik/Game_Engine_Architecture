#include "resourcesystem.h"

resourceSystem::resourceSystem()
{

}

void resourceSystem::CreateMeshComponent(std::string input, MeshComponent * mesh){
    //qDebug() << "-------------------------------------------Allocating And Setting Component Mesh--------------------------------";

    for(unsigned long long i = 0; i < meshDataContainer.size(); i++){

        if(input == meshDataContainer[i].first)
        {

            mesh->collisionRadius = meshDataContainer[i].second.collisionRadius;
            mesh->mVAO[0] = meshDataContainer[i].second.VAO;
            mesh->mVBO[0] = meshDataContainer[i].second.VBO;
            mesh->VertexSize[0] = meshDataContainer[i].second.meshVert.size();
            mesh->LODEnabled = CheckLOD12Presence(meshDataContainer[i].first);
            mesh->meshName = input;
            if(mesh->LODEnabled)
            {

                std::string sLOD1 = getPureName(meshDataContainer[i].first) + "_L01.obj", sLOD2 = getPureName(meshDataContainer[i].first) + "_L02.obj";

                for(unsigned long long k = 0; k < meshDataContainer.size(); k++){
                    if(sLOD1 == meshDataContainer[k].first)
                    {

                        mesh->mVAO[1] = meshDataContainer[k].second.VAO;
                        mesh->mVBO[1] = meshDataContainer[k].second.VBO;
                        mesh->VertexSize[1] = meshDataContainer[k].second.meshVert.size();

                    }
                    if(sLOD2 == meshDataContainer[k].first)
                    {

                        mesh->mVAO[2] = meshDataContainer[k].second.VAO;
                        mesh->mVBO[2] = meshDataContainer[k].second.VBO;
                        mesh->VertexSize[2] = meshDataContainer[k].second.meshVert.size();
                    }
                }
            }

            break;
        }
    }

}



std::vector<std::string> resourceSystem::GetAllMeshesInAssetsDirectory()
{
    std::vector<std::string> assetNames;
    std::string dir = gsl::ModelFilePath;
    // "-------------------------------------------Checking Model Assets--------------------------------";
    QDirIterator iterator("../GEA2021/Assets/Models/", QDirIterator::Subdirectories);
    while (iterator.hasNext())
    {
        QFile file(iterator.next());
        if ( file.open( QIODevice::ReadOnly ) )
        {
            QFileInfo fileInf(file.fileName());
            QString qFN = fileInf.fileName();
            std::string fn = qFN.toStdString();
            if(fn.find("_L0") != std::string::npos)
            {
                qDebug() << "Detected LOD Model :" << fileInf.fileName();
            }else{
                qDebug() << "Detected Model :" << fileInf.fileName();
            }
            assetNames.push_back(fn);
        }
    }
    //"-----------------------------------------------Completed----------------------------------------";

    return assetNames;
}

void resourceSystem::ResourceSystemInit(RenderSystem * inRendSys)
{
    rendSys = inRendSys;
    SetMeshDataContainer();
    // "-------------------------------------------Loading Model Assets---------------------------------";
    for(unsigned long long i = 0; i < meshDataContainer.size(); i++)
    {
        rendSys->init(&meshDataContainer[i].second.meshVert, &meshDataContainer[i].second.VAO, &meshDataContainer[i].second.VBO);
    }
    // "-----------------------------------------------Completed----------------------------------------";
}

void resourceSystem::SetMeshDataContainer()
{
    meshDataContainer.clear();
    meshData obj;
    std::vector<Vertex> mVertices;
    mVertices.push_back(Vertex{0.f, 0.f, 0.f, 1.f, 0.f, 0.f});
    mVertices.push_back(Vertex{100.f, 0.f, 0.f, 1.f, 0.f, 0.f});
    mVertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 1.f, 0.f});
    mVertices.push_back(Vertex{0.f, 100.f, 0.f, 0.f, 1.f, 0.f});
    mVertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 0.f, 1.f});
    mVertices.push_back(Vertex{0.f, 0.f, 100.f, 0.f, 0.f, 1.f});
    obj.meshVert = mVertices;
    obj.DrawType = GL_LINES;
    obj.VAO = 0;
    obj.internalIndex = 1;
    meshDataContainer.push_back(std::make_pair("XYZ", obj));

    std::vector<std::string> assetNames = GetAllMeshesInAssetsDirectory();

    for(unsigned long long i = 0; i < assetNames.size(); i++){
        meshData obj;
        std::string filename = gsl::ModelFilePath + assetNames[i];
        std::vector<Vertex> tempMVertices;
        std::vector<GLuint> tempMIndices;
        float tempRadius = 0;

        //Open File
        std::ifstream fileIn;
        fileIn.open (filename, std::ifstream::in);
        if(!fileIn)
            qDebug() << "Could not open file for reading: " << QString::fromStdString(filename);

        //One line at a time-variable
        std::string oneLine;
        //One word at a time-variable
        std::string oneWord;

        std::vector<QVector3D> tempVertecies;
        std::vector<QVector3D> tempNormals;
        std::vector<QVector2D> tempUVs;
        std::vector<QVector3D> tempColors;


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
                //            //qDebug() << "Line is comment "  << QString::fromStdString(oneWord);
                continue;
            }
            if (oneWord == "")
            {
                //Ignore this line
                //            //qDebug() << "Line is blank ";
                continue;
            }
            if (oneWord == "v")
            {
                //            //qDebug() << "Line is vertex "  << QString::fromStdString(oneWord) << " ";
                QVector3D tempVertex;
                sStream >> oneWord;
                tempVertex.setX(std::stof(oneWord));
                sStream >> oneWord;
                tempVertex.setY( std::stof(oneWord));
                sStream >> oneWord;
                tempVertex.setZ (std::stof(oneWord));

                //Vertex made - pushing it into vertex-vector
                tempVertecies.push_back(tempVertex);

                //calculate radius of sphere
                tempRadius = calculateLenght(tempVertex);
                //if last radius is less than new radius, apply new radius
                if(obj.collisionRadius < tempRadius)
                    obj.collisionRadius = tempRadius;

                continue;
            }
            if (oneWord == "vt")
            {
                //            //qDebug() << "Line is UV-coordinate "  << QString::fromStdString(oneWord) << " ";
                QVector2D tempUV;
                sStream >> oneWord;
                tempUV.setX( std::stof(oneWord));
                sStream >> oneWord;
                tempUV.setY( std::stof(oneWord));

                //UV made - pushing it into UV-vector
                tempUVs.push_back(tempUV);

                continue;
            }
            if (oneWord == "vn")
            {
                //            //qDebug() << "Line is normal "  << QString::fromStdString(oneWord) << " ";
                QVector3D tempNormal;
                sStream >> oneWord;
                tempNormal.setX( std::stof(oneWord));
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
                        tempMVertices.push_back(tempVert);
                    }
                    else            //no uv in mesh data, use 0, 0 as uv
                    {
                        Vertex tempVert(tempVertecies[index], tempNormals[normal], QVector2D(0.0f, 0.0f));
                        tempMVertices.push_back(tempVert);
                    }
                    tempMIndices.push_back(temp_index++);
                }
                continue;
            }
        }
        //beeing a nice boy and closing the file after use
        fileIn.close();
        //copy the object for future use
        obj.meshVert = tempMVertices;
        obj.meshIndic = tempMIndices;
        obj.collisionRadius = tempRadius;
        obj.internalIndex = static_cast<int>(meshDataContainer.size()-1);
        obj.VAO = 0;//(GLuint)meshDataContainer.size()+1;
        obj.VBO = 0;//(GLuint)meshDataContainer.size()+1;
        meshDataContainer.push_back(std::make_pair(assetNames[i], obj));

    }
}

void resourceSystem::SetIntoMeshDataContainerRUNTIME(std::vector<Vertex> mVertices, std::string fname)
{
    meshData obj;
    //obj.meshVert = mVertices;
    for(unsigned long long i = 0; i< mVertices.size() - 3; i++)
    {
        obj.meshVert.push_back( mVertices[i]);
    }
    obj.internalIndex = static_cast<int>(meshDataContainer.size()-1);
    obj.VAO = 0;
    obj.VBO = 0;
    obj.DrawType = GL_POINTS;
    meshDataContainer.push_back(std::make_pair(fname, obj));
    rendSys->init(&meshDataContainer[meshDataContainer.size()-1].second.meshVert, &meshDataContainer[meshDataContainer.size()-1].second.VAO, &meshDataContainer[meshDataContainer.size()-1].second.VBO);
}

// Returns vertex data for a specific mesh.
std::vector<Vertex> resourceSystem::getVertexDataByName(std::string meshName)
{
    for(unsigned long long i = 0; i < meshDataContainer.size(); i++)
    {
        if(meshName == meshDataContainer[i].first)
        {
            return meshDataContainer[i].second.meshVert;
        }
    }
}

bool resourceSystem::CheckLOD12Presence(std::string meshName)
{
    std::string modifiedMeshName = getPureName(meshName);
    bool bLOD1 = false, bLOD2 = false;
    std::string sLOD1 = modifiedMeshName+"_L01.obj", sLOD2 = modifiedMeshName+"_L02.obj";
    for(unsigned long long i = 0; i < meshDataContainer.size(); i++)
    {
        if(sLOD1 == meshDataContainer[i].first)
        {
            bLOD1 = true;
            //qDebug()<<"LOD1 DETECTED:" << QString::fromStdString(sLOD1);
        }
        if(sLOD2 == meshDataContainer[i].first)
        {
            //qDebug()<<"LOD2 DETECTED:" << QString::fromStdString(sLOD2);
            bLOD2 = true;
        }
    }

    if(bLOD1 == true && bLOD2 == true)
    {
        //qDebug()<<"LOD REQUIREMENTS MET FOR " << QString::fromStdString(meshName);
        return true;
    }
    else
    {
        //qDebug()<<"LOD REQUIREMENTS NOT MET FOR" << QString::fromStdString(meshName) << " @ " << QString::fromStdString(sLOD1) << " AND " << QString::fromStdString(sLOD2);
    }
    return false;
}

std::string resourceSystem::getPureName(std::string objMeshName)
{
    std::string modifiedMeshName;
    for(unsigned long long i = 0; i < objMeshName.size(); i++)
    {
        if(objMeshName[i] != '.')
        {
            modifiedMeshName += objMeshName[i];
        }
        else
        {
            break;
        }
    }
    return modifiedMeshName;
}



meshData* resourceSystem::makeFrustum(const Frustum &frustumIn, RenderSystem * inRendSys)
{
    //calculate corners of frustum:
    //Math shown here: https://learnopengl.com/Guest-Articles/2021/Scene/Frustum-Culling
    //code taken from ole.experiment
    float tanFOVv = tanf(gsl::deg2radf(frustumIn.mFOVvertical/2));          // expensive calculation - save answer

    float halfHeightFar = abs(frustumIn.mFarPlaneDistance * tanFOVv);
    float halfWidthFar = halfHeightFar * frustumIn.mAspectRatio;

    float halfHeightNear = abs(frustumIn.mNearPlaneDistance * tanFOVv);
    float halfWidthNear = halfHeightNear * frustumIn.mAspectRatio;

    // camera looks down -Z (as a start) so near and far-plane are negative when drawn
    gsl::Vector3D cornerNear = gsl::Vector3D(halfWidthNear, halfHeightNear, -frustumIn.mNearPlaneDistance);
    gsl::Vector3D cornerFar = gsl::Vector3D(halfWidthFar, halfHeightFar, -frustumIn.mFarPlaneDistance);

    meshData *tempMesh = new meshData();
    //Vertex data for front points                  color                       uv
    tempMesh->meshVert.push_back(Vertex(-cornerNear.getX(), -cornerNear.getY(), cornerNear.getZ(),       1.f, 0.301f, 0.933f,          0.f, 0.f));//0
    tempMesh->meshVert.push_back(Vertex(cornerNear.getX(),  -cornerNear.getY(), cornerNear.getZ(),       1.f, 0.301f, 0.933f,          0.f, 0.f));//1
    tempMesh->meshVert.push_back(Vertex(cornerNear.getX(),  -cornerNear.getY(), cornerNear.getZ(),       1.f, 0.301f, 0.933f,          0.f, 0.f));//1
    tempMesh->meshVert.push_back(Vertex(cornerNear.getX(),  cornerNear.getY(),  cornerNear.getZ(),       1.f, 0.301f, 0.933f,          0.f, 0.f));//2
    tempMesh->meshVert.push_back(Vertex(cornerNear.getX(),  cornerNear.getY(),  cornerNear.getZ(),       1.f, 0.301f, 0.933f,          0.f, 0.f));//2
    tempMesh->meshVert.push_back(Vertex(-cornerNear.getX(), cornerNear.getY(),  cornerNear.getZ(),       1.f, 0.301f, 0.933f,          0.f, 0.f));//3
    tempMesh->meshVert.push_back(Vertex(-cornerNear.getX(), cornerNear.getY(),  cornerNear.getZ(),       1.f, 0.301f, 0.933f,          0.f, 0.f));//3
    tempMesh->meshVert.push_back(Vertex(-cornerNear.getX(), -cornerNear.getY(), cornerNear.getZ(),       1.f, 0.301f, 0.933f,          0.f, 0.f));//0
    //Vertex data for back
    tempMesh->meshVert.push_back(Vertex(-cornerFar.getX(), -cornerFar.getY(), cornerFar.getZ(),          1.f, 0.301f, 0.933f,          0.f, 0.f));//4
    tempMesh->meshVert.push_back(Vertex(cornerFar.getX(),  -cornerFar.getY(), cornerFar.getZ(),          1.f, 0.301f, 0.933f,          0.f, 0.f));//5
    tempMesh->meshVert.push_back(Vertex(cornerFar.getX(),  -cornerFar.getY(), cornerFar.getZ(),          1.f, 0.301f, 0.933f,          0.f, 0.f));//5
    tempMesh->meshVert.push_back(Vertex(cornerFar.getX(),  cornerFar.getY(),  cornerFar.getZ(),          1.f, 0.301f, 0.933f,          0.f, 0.f));//6
    tempMesh->meshVert.push_back(Vertex(cornerFar.getX(),  cornerFar.getY(),  cornerFar.getZ(),          1.f, 0.301f, 0.933f,          0.f, 0.f));//6
    tempMesh->meshVert.push_back(Vertex(-cornerFar.getX(), cornerFar.getY(),  cornerFar.getZ(),          1.f, 0.301f, 0.933f,          0.f, 0.f));//7
    tempMesh->meshVert.push_back(Vertex(-cornerFar.getX(), cornerFar.getY(),  cornerFar.getZ(),          1.f, 0.301f, 0.933f,          0.f, 0.f));//7
    tempMesh->meshVert.push_back(Vertex(-cornerFar.getX(), -cornerFar.getY(), cornerFar.getZ(),          1.f, 0.301f, 0.933f,          0.f, 0.f));//4

    tempMesh->meshVert.push_back(Vertex(-cornerNear.getX(), -cornerNear.getY(), cornerNear.getZ(),       1.f, 0.301f, 0.933f,          0.f, 0.f));//0
    tempMesh->meshVert.push_back(Vertex(-cornerFar.getX(), -cornerFar.getY(), cornerFar.getZ(),          1.f, 0.301f, 0.933f,          0.f, 0.f));//4
    tempMesh->meshVert.push_back(Vertex(-cornerNear.getX(), cornerNear.getY(),  cornerNear.getZ(),       1.f, 0.301f, 0.933f,          0.f, 0.f));//3
    tempMesh->meshVert.push_back(Vertex(-cornerFar.getX(), cornerFar.getY(),  cornerFar.getZ(),          1.f, 0.301f, 0.933f,          0.f, 0.f));//7

    tempMesh->meshVert.push_back(Vertex(cornerNear.getX(),  -cornerNear.getY(), cornerNear.getZ(),       1.f, 0.301f, 0.933f,          0.f, 0.f));//1
    tempMesh->meshVert.push_back(Vertex(cornerFar.getX(),  -cornerFar.getY(), cornerFar.getZ(),          1.f, 0.301f, 0.933f,          0.f, 0.f));//5
    tempMesh->meshVert.push_back(Vertex(cornerNear.getX(),  cornerNear.getY(),  cornerNear.getZ(),       1.f, 0.301f, 0.933f,          0.f, 0.f));//2
    tempMesh->meshVert.push_back(Vertex(cornerFar.getX(),  cornerFar.getY(),  cornerFar.getZ(),          1.f, 0.301f, 0.933f,          0.f, 0.f));//6

    //One line at a time
    tempMesh->meshIndic.clear();
    unsigned int arr[24]=
    { 0, 1, 1, 2, 2, 3, 3, 0,       //front rectangle
      4, 5, 5, 6, 6, 7, 7, 4,       //back rectangle
      0, 4, 3, 7,                   //leftside lines
      1, 5, 2, 6                    //rightside lines
    };
    for (unsigned long long i = 0; i < sizeof (arr); i++)
    {
        tempMesh->meshIndic.push_back(arr[i]);
    }

    tempMesh->DrawType = GL_LINES;
    tempMesh->VAO = 0;
    tempMesh->VBO = 0;    //only LOD level 0
    inRendSys->init(&tempMesh->meshVert, &tempMesh->VAO, &tempMesh->VBO);

    //initMesh(tempMesh, 0);
    tempMesh->internalIndex = tempMesh->meshIndic.size();
    return tempMesh;
}

meshData* resourceSystem::makeSkyBox( RenderSystem * inRendSys)
{


    meshData *tempMesh = new meshData();
    //vertex for cube https://learnopengl.com/code_viewer.php?code=advanced/cubemaps_skybox_data
    std::vector<Vertex> mVertices;
    mVertices.push_back(Vertex{-1.0f,  1.0f, -1.0f });
    mVertices.push_back(Vertex{-1.0f, -1.0f, -1.0f });
    mVertices.push_back(Vertex{ 1.0f, -1.0f, -1.0f });
    mVertices.push_back(Vertex{ 1.0f, -1.0f, -1.0f });
    mVertices.push_back(Vertex{ 1.0f,  1.0f, -1.0f });
    mVertices.push_back(Vertex{-1.0f,  1.0f, -1.0f });

    mVertices.push_back(Vertex{-1.0f, -1.0f,  1.0f});
    mVertices.push_back(Vertex{-1.0f, -1.0f, -1.0f});
    mVertices.push_back(Vertex{-1.0f,  1.0f, -1.0f});
    mVertices.push_back(Vertex{-1.0f,  1.0f, -1.0f});
    mVertices.push_back(Vertex{-1.0f,  1.0f,  1.0f});
    mVertices.push_back(Vertex{-1.0f, -1.0f,  1.0f});

    mVertices.push_back(Vertex{1.0f, -1.0f, -1.0f});
    mVertices.push_back(Vertex{1.0f, -1.0f,  1.0f});
    mVertices.push_back(Vertex{1.0f,  1.0f,  1.0f});
    mVertices.push_back(Vertex{1.0f,  1.0f,  1.0f});
    mVertices.push_back(Vertex{1.0f,  1.0f, -1.0f});
    mVertices.push_back(Vertex{1.0f, -1.0f, -1.0f});

    mVertices.push_back(Vertex{-1.0f, -1.0f,  1.0f});
    mVertices.push_back(Vertex{-1.0f,  1.0f,  1.0f});
    mVertices.push_back(Vertex{ 1.0f,  1.0f,  1.0f});
    mVertices.push_back(Vertex{ 1.0f,  1.0f,  1.0f});
    mVertices.push_back(Vertex{ 1.0f, -1.0f,  1.0f});
    mVertices.push_back(Vertex{-1.0f, -1.0f,  1.0f});

    mVertices.push_back(Vertex{-1.0f,  1.0f, -1.0f});
    mVertices.push_back(Vertex{ 1.0f,  1.0f, -1.0f});
    mVertices.push_back(Vertex{ 1.0f,  1.0f,  1.0f});
    mVertices.push_back(Vertex{ 1.0f,  1.0f,  1.0f});
    mVertices.push_back(Vertex{-1.0f,  1.0f,  1.0f});
    mVertices.push_back(Vertex{-1.0f,  1.0f, -1.0f});

    mVertices.push_back(Vertex{-1.0f, -1.0f, -1.0});
    mVertices.push_back(Vertex{-1.0f, -1.0f,  1.0});
    mVertices.push_back(Vertex{ 1.0f, -1.0f, -1.0});
    mVertices.push_back(Vertex{ 1.0f, -1.0f, -1.0});
    mVertices.push_back(Vertex{-1.0f, -1.0f,  1.0});
    mVertices.push_back(Vertex{ 1.0f, -1.0f,  1.0});

    tempMesh->meshVert = mVertices;


    tempMesh->DrawType = GL_TRIANGLES;
    tempMesh->VAO = 0;
    tempMesh->VBO = 0;    //only LOD level 0
    inRendSys->init(&tempMesh->meshVert, &tempMesh->VAO, &tempMesh->VBO);

    //initMesh(tempMesh, 0);
    tempMesh->internalIndex = tempMesh->meshIndic.size();
    return tempMesh;
}

float resourceSystem::calculateLenght(QVector3D pos)
{   //we assume that the center of the obj is at 0,0,0
    //sqrt(x^2        +         y^2        +         z^2      ) = length
    return sqrt((pos.x()*pos.x()) + (pos.y()* pos.y()) + (pos.z()*pos.z()));
}


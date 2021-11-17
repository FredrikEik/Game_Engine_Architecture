#include "ResourceManager.h"

#include "renderwindow.h"
//#include <sstream>

ResourceManager::ResourceManager()
{
    mCoreEngine = CoreEngine::getInstance();
}

GameObject* ResourceManager::CreateMainCharacter(std::string filename)
{
    //makeMainCharacter
    object = CreateObject(filename);
    object->transform->mMatrix.translate(57.f, -1.f, 6.f); //Input character start position.

    return object;
}
GameObject* ResourceManager::CreateObject(std::string filename)
{
    object = new GameObject();
    object->transform = new Transform();
    object->transform->mMatrix.setToIdentity();
    object->material = new Material();
    object->mesh = new Mesh();

    object->mName = filename;

    int meshIndex{-1};
    auto result = mMeshIndexMap.find(filename);

    if(result != mMeshIndexMap.end()){
        meshIndex = result->second;

      //Gives the current objects mesh the same values as the "duplicate" version. (Updates mVAO, drawType, etc.)
        object->mesh = &mMeshComponents.at(meshIndex);
    }
    else{

        if(filename.find(".obj") != std::string::npos)
            meshIndex = readObj(gsl::MeshFilePath + filename);

        if(filename.find(".txt") != std::string::npos) // only for .txt files with heightInfo
            meshIndex = makeHeightMapFromTxt(gsl::HeightInfoPath + filename);

        if (filename.find(".bmp") != std::string::npos)
            meshIndex = makeHeightMap(filename);

        mMeshIndexMap.emplace(filename, meshIndex);
        mMeshComponents[meshIndex] = *object->mesh;
    }

    if(meshIndex == -1)
        std::cout << "Error: meshIndex is -1 (no mesh)";

    if(object->mesh->mVertices[1].size() == 0) //If this is true, the object has no LOD variants
        object->mesh->bLodEnabled = false;



    return object;
}


ResourceManager &ResourceManager::getInstance()
{
    static ResourceManager* mInstance = new ResourceManager();
    return *mInstance;
}

void ResourceManager::init(Mesh &meshComp, int lod)
{
    initializeOpenGLFunctions();

    glGenVertexArrays( 1, &meshComp.mVAO[lod] );
    glBindVertexArray( meshComp.mVAO[lod] );

    glGenBuffers( 1, &meshComp.mVBO[lod]);
    glBindBuffer( GL_ARRAY_BUFFER, meshComp.mVBO[lod] );

    glBufferData( GL_ARRAY_BUFFER, meshComp.mVertices[lod].size()*sizeof( Vertex ), meshComp.mVertices[lod].data(), GL_STATIC_DRAW );

    // 1st attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, meshComp.mVBO[lod]);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors / Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    if(meshComp.mIndices[lod].size() > 0) {
        glGenBuffers(1, &meshComp.mEAB[lod]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshComp.mEAB[lod]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshComp.mIndices[lod].size() * sizeof(GLuint), meshComp.mIndices[lod].data(), GL_STATIC_DRAW);
    }

    glBindVertexArray(0);
}

float ResourceManager::getHeightMapHeight(const gsl::Vector2D &pos)
{
    gsl::Matrix4x4 hmMatrix = mCoreEngine->getHeightMapMatrix();

    float xPosOnTerrain = (pos.x - hmMatrix.getPosition().x);
    float yPosOnTerrain = (pos.y - hmMatrix.getPosition().y);
    float gridSquareSize = xyScale;
    //hvilken grid spilleren er på
    int gridXPos = int(floor(xPosOnTerrain / xyScale));
    int gridYPos = int(floor(yPosOnTerrain / xyScale));
    if(gridXPos >= 0 && gridYPos >= 0 && gridXPos < cols && gridYPos < rows)
    {   //Koordinat grid
        float xCoordInSquare = fmod(xPosOnTerrain,gridSquareSize)/gridSquareSize;
        float yCoordInSquare = fmod(yPosOnTerrain,gridSquareSize)/gridSquareSize;
        gsl::Vector3D uvw;
        float answer;
        gsl::Vector2D p1{0,0};
        gsl::Vector2D p2{1,0};
        gsl::Vector2D p3{0,1};
        gsl::Vector2D p4{1,1};
        float p1z = sArrayHeights[(gridYPos)*rows + (gridXPos)];
        float p2z = sArrayHeights[(gridYPos+1)*rows + (gridXPos)];
        float p3z = sArrayHeights[(gridYPos)*rows + (gridXPos+1)];
        float p4z = sArrayHeights[(gridYPos+1)*rows + (gridXPos+1)];


        //Finne hvilken av de to trekantene spilleren står på
        if(xCoordInSquare <= (1-yCoordInSquare))
        {
            uvw = barycentricCoordinates(gsl::Vector2D{float(xCoordInSquare),float(yCoordInSquare)},p1,p2,p3);
            answer = p1z*uvw.x+p2z*uvw.y+p3z*uvw.z;
            return answer;
        }
        if(xCoordInSquare > (1-yCoordInSquare))
        {   gsl::Vector2D posInsSquare{xCoordInSquare,yCoordInSquare};
            uvw = barycentricCoordinates(posInsSquare,p2,p4,p3);
            answer = p2z*uvw.x+p4z*uvw.y+p3z*uvw.z;
            return answer;
        }
    }
    return NULL;
}

int ResourceManager::makeHeightMapFromTxt(std::string filename)
{
    mMeshComponents.emplace_back(Mesh());
    *object->mesh = mMeshComponents.back();

    std::vector<gsl::Vector3D> tempPos;
    std::vector<gsl::Vector3D> sortedPos;

    std::ifstream inn;
    inn.open(filename.c_str());

    if (inn.is_open())
    {
        int n; //antall punkter?
        inn >> n;

        std::string coord = "";

        float x = 0.0f, y = 0.0f, z = 0.0f;
        float xMin = 0.0f, xMax = 0.0f, yMin = 0.0f, yMax = 0.0f;

        const int rows = 50;
        const int cols = 50;

    // Vet ikke om jeg trenger disse.
        object->mesh->mVertices[0].reserve(n);
        tempPos.reserve(n);
        sortedPos.reserve(n);

        for (int i=0; i<n; i++)
        {
            inn >> x >> y >> z;

            tempPos.push_back(gsl::Vector3D{x, y, z});

//if i == 0, xMin and yMin is 0. result would be xMin and yMin == 0 at the end of loop
            if(x < xMin || i == 0)
                xMin = x;
            if(y < yMin || i == 0)
                yMin = y;
            if(x > xMax || i == 0)
                xMax = x;
            if(y > yMax || i == 0)
                yMax = y;
        }
        float xStep = (xMax-xMin)/rows; //avstanden mellom hvert hjørne i et kvadrant (i x-planet)
        float yStep = (yMax-yMin)/cols; //avstanden mellom hvert hjørne i et kvadrant (i y-planet)
        int vertexesInQuad[rows][cols]; //antall vertexer i kvadrant [x][y]
        float tempForAvg[rows][cols];   //holder summen av z verdiene i kvadrant [x][y]

        for (int y=0; y<cols; y++){
            for (int x=0; x<rows; x++) {
                tempForAvg[x][y]=0.0f;
                vertexesInQuad[x][y]=0;
             }
        }
        for (int i = 0; i < tempPos.size(); i++)
        {
//            int x1 = (int)((tempPos[i].x-xMin) / xStep);
//            int y1 = (int)((tempPos[i].y-yMin) / yStep);
            int x1 = static_cast<int>((tempPos[i].x-xMin) / xStep);
            int y1 = static_cast<int>((tempPos[i].y-yMin) / yStep);
            tempForAvg[x1][y1] += tempPos[i].z;
            vertexesInQuad[x1][y1]++;
        }

        float xtemp;
        float ytemp = 540;
        float ztemp;

        for (int y=0; y<cols; y++)
        {
            for (int x=0; x<rows; x++)
            {
                xtemp = xMin+(x*xStep)+(xStep/2);
                ztemp = yMin+(y*yStep)+(yStep/2);

                if(tempForAvg[x][y] != 0)
                {
                    ytemp = tempForAvg[x][y] / vertexesInQuad[x][y];
                    sortedPos.push_back(gsl::Vector3D{ xtemp, ytemp, ztemp});
                }
                else
                    sortedPos.push_back(gsl::Vector3D{ xtemp, ytemp, ztemp});

            }
        }
        for(int i = 0; i<sortedPos.size()-rows-1; i++)
        {
            //qDebug() << sortedPos[i].x << sortedPos[i].y << sortedPos[i].z;
            object->mesh->mVertices[0].push_back(Vertex{sortedPos[i].x, sortedPos[i].y, sortedPos[i].z, 0,0.5,0});
            object->mesh->mVertices[0].push_back(Vertex{sortedPos[i+1].x, sortedPos[i+1].y, sortedPos[i+1].z, 0,0.5,0});
            object->mesh->mVertices[0].push_back(Vertex{sortedPos[i+rows].x, sortedPos[i+rows].y, sortedPos[i+rows].z, 0,0.5,0});

            object->mesh->mVertices[0].push_back(Vertex{sortedPos[i+rows].x, sortedPos[i+rows].y, sortedPos[i+rows].z, 0,0.5,0});
            object->mesh->mVertices[0].push_back(Vertex{sortedPos[i+1].x, sortedPos[i+1].y, sortedPos[i+1].z, 0,0.5,0});
            object->mesh->mVertices[0].push_back(Vertex{sortedPos[i+rows+1].x, sortedPos[i+rows+1].y, sortedPos[i+rows+1].z, 0,0.5,0});
        }

    /** Create Indices */

//        int c=0;
//        //Pusher inn indeksene i riktig rekkefølge slik at den tegner 1 quad av 2 trekanter per vertex, men ikke på den øverste raden.
//        for(GLuint i{}; i<object->mesh->mVertices[0].size()-rows-1;i++)
//        {
//            //Sjekk for å ungå å tegne en utstrekt skrå quad fra slutten på raden til starten på neste rad
//            if(c == rows-1)
//            {
//                c=0;
//                continue;
//            }
//            object->mesh->mIndices[0].push_back(i);
//            object->mesh->mIndices[0].push_back(i+1);
//            object->mesh->mIndices[0].push_back(i+rows);

//            object->mesh->mIndices[0].push_back(i+rows);
//            object->mesh->mIndices[0].push_back(i+1);
//            object->mesh->mIndices[0].push_back(i+rows+1);
//            c++;
//        }

    /** Make Normals */

//        gsl::Vector3D pCenter,p0,p1,p2,p3,p4,p5;
//        gsl::Vector3D n0,n1,n2,n3,n4,n5;

//        std::vector<Vertex> vert = object->mesh->mVertices[0];

//        for(int i = 1; i<object->mesh->mVertices[0].size()-rows; i++)
//        {
//            gsl::Vector3D pos = vert[i].mXYZ;

//            if(pos.x > 0 && pos.y > 0 /*&& pos.z > 0*/)
//            {
//                pCenter = gsl::Vector3D{pos.x, pos.y, pos.z};

//                //p0
//                p0 = gsl::Vector3D{vert[i-rows].mXYZ.x, vert[i-rows].mXYZ.y, vert[i-rows].mXYZ.z};

//                //p1
//                p1 = gsl::Vector3D{vert[i+1-rows].mXYZ.x, vert[i+1-rows].mXYZ.y, vert[i+1-rows].mXYZ.z};

//                //p2
//                p2 = gsl::Vector3D{vert[i+1].mXYZ.x, vert[i+1].mXYZ.y, vert[i+1].mXYZ.z};

//                //p3
//                p3 = gsl::Vector3D{vert[i+rows].mXYZ.x, vert[i+rows].mXYZ.y, vert[i+rows].mXYZ.z};

//                //p4
//                p4 = gsl::Vector3D{vert[i-1+rows].mXYZ.x, vert[i-1+rows].mXYZ.y, vert[i-1+rows].mXYZ.z};

//                //p5
//                p5 = gsl::Vector3D{vert[i-1].mXYZ.x, vert[i-1].mXYZ.y, vert[i-1].mXYZ.z};
//            }
//            //lager vektorer til alle punktene
//            gsl::Vector3D v0 = p0-pCenter;
//            gsl::Vector3D v1 = p1-pCenter;
//            gsl::Vector3D v2 = p2-pCenter;
//            gsl::Vector3D v3 = p3-pCenter;
//            gsl::Vector3D v4 = p4-pCenter;
//            gsl::Vector3D v5 = p5-pCenter;

//            //Regner ut normalene til alle trekantene rundt punktet

//            n0 = v0.cross(v0,v1);
//            n0.normalize();

//            n1 = v1.cross(v1,v2);
//            n1.normalize();

//            n2 = v2.cross(v2,v3);
//            n2.normalize();

//            n3 = v3.cross(v3,v4);
//            n3.normalize();

//            n4 = v4.cross(v4,v5);
//            n4.normalize();

//            n5 = v5.cross(v5,v0);
//            n5.normalize();

//            gsl::Vector3D nV = n0+n1+n2+n3+n4+n5;

//            vert[i].mNormal.x = nV.x;
//            vert[i].mNormal.y = nV.y;
//            vert[i].mNormal.z = nV.z;
//        }

        inn.close();
    }
    init(*object->mesh, 0);

    return mMeshComponents.size()-1;
}

int ResourceManager::makeHeightMap(std::string filename)
{
    texture = new Texture(filename, false);


    rows = texture->mRows;   //WARNING: only using half of the heightmap. (Because of lag issues)
    cols = texture->mColumns;

    unsigned char *mHeights = texture->getmHeights();
    int bytesPrPixel = texture->getBytesPerPixel();
    float zColorScale = 256.f;
    float zScale = 0.02f;
    int i = 0;
    int j = 0;
    sArrayHeights = new float[rows*cols];
    if(!mHeights)
    {
        qDebug() << "'!mHeights' in ResourceManager::makeHeightMap";
        return NULL;
    }
    //Pusher inn alle vertexene med riktig høyde
    for (int y = 0;y<cols;y++){
        for (int x = 0;x<rows;x++)
        {
            float z = float(mHeights[i]);
            object->mesh->mVertices[0].push_back(Vertex{x*xyScale, y*xyScale, z*zScale, z/zColorScale, z/zColorScale, z/zColorScale,0,0});
            //alle høydene i en array som jeg skal bruke til barysentriske kooridinater
            sArrayHeights[j] = z;
            j++;
            i+=bytesPrPixel;
        }
    }

    int squareCounter = 0;
    int indexCounter = 0;
    float XpartOfEight = 0;
    float YpartOfEight = 0;
    for(int y = 0;y<cols;y++){
        for(int x=0; x<rows;x++)
        {
            object->mesh->mVertices[0][indexCounter].mST.x = XpartOfEight/7;
            object->mesh->mVertices[0][indexCounter].mST.y = YpartOfEight/7;
            indexCounter++;
            squareCounter++;
            XpartOfEight+=1;
            if(XpartOfEight>rows-1)
            {
                XpartOfEight = 0;
            }
        }
        YpartOfEight+=1;
        if(YpartOfEight>rows-1)
        {
            YpartOfEight = 0;
        }
    }

    int c=0;
    //Pusher inn indeksene i riktig rekkefølge slik at den tegner 1 quad av 2 trekanter per vertex, men ikke på den øverste raden.
    for(GLuint i{}; i<object->mesh->mVertices[0].size()-rows-1;i++)
    {
        //Sjekk for å ungå å tegne en utstrekt skrå quad fra slutten på raden til starten på neste rad
        if(c == rows-1)
        {
            c=0;
            continue;
        }
        object->mesh->mIndices[0].push_back(i);
        object->mesh->mIndices[0].push_back(i+1);
        object->mesh->mIndices[0].push_back(i+rows);

        object->mesh->mIndices[0].push_back(i+rows);
        object->mesh->mIndices[0].push_back(i+1);
        object->mesh->mIndices[0].push_back(i+rows+1);
        c++;
    }

    //Normals:
    gsl::Vector3D pCenter,p0,p1,p2,p3,p4,p5;
    gsl::Vector3D n0,n1,n2,n3,n4,n5;

    std::vector<Vertex> vert = object->mesh->mVertices[0];

    for(int i = 1;i<object->mesh->mVertices[0].size()-rows;i++)
    {
        gsl::Vector3D pos = vert[i].mXYZ;

        if(pos.x > 0 && pos.y > 0 /*&& pos.z > 0*/)
        {
            pCenter = gsl::Vector3D{pos.x, pos.y, pos.z};

            //p0
            p0 = gsl::Vector3D{vert[i-rows].mXYZ.x, vert[i-rows].mXYZ.y, vert[i-rows].mXYZ.z};

            //p1
            p1 = gsl::Vector3D{vert[i+1-rows].mXYZ.x, vert[i+1-rows].mXYZ.y, vert[i+1-rows].mXYZ.z};

            //p2
            p2 = gsl::Vector3D{vert[i+1].mXYZ.x, vert[i+1].mXYZ.y, vert[i+1].mXYZ.z};

            //p3
            p3 = gsl::Vector3D{vert[i+rows].mXYZ.x, vert[i+rows].mXYZ.y, vert[i+rows].mXYZ.z};

            //p4
            p4 = gsl::Vector3D{vert[i-1+rows].mXYZ.x, vert[i-1+rows].mXYZ.y, vert[i-1+rows].mXYZ.z};

            //p5
            p5 = gsl::Vector3D{vert[i-1].mXYZ.x, vert[i-1].mXYZ.y, vert[i-1].mXYZ.z};
        }
        //lager vektorer til alle punktene
        gsl::Vector3D v0 = p0-pCenter;
        gsl::Vector3D v1 = p1-pCenter;
        gsl::Vector3D v2 = p2-pCenter;
        gsl::Vector3D v3 = p3-pCenter;
        gsl::Vector3D v4 = p4-pCenter;
        gsl::Vector3D v5 = p5-pCenter;

        //Regner ut normalene til alle trekantene rundt punktet

        n0 = v0.cross(v0,v1);
        n0.normalize();

        n1 = v1.cross(v1,v2);
        n1.normalize();

        n2 = v2.cross(v2,v3);
        n2.normalize();

        n3 = v3.cross(v3,v4);
        n3.normalize();

        n4 = v4.cross(v4,v5);
        n4.normalize();

        n5 = v5.cross(v5,v0);
        n5.normalize();

        gsl::Vector3D nV = n0+n1+n2+n3+n4+n5;

        vert[i].mNormal.x = nV.x;
        vert[i].mNormal.y = nV.y;
        vert[i].mNormal.z = nV.z;
    }
    init(*object->mesh, 0);

    return mMeshComponents.size()-1;    //returns index to last object
}

gsl::Vector3D ResourceManager::barycentricCoordinates(const gsl::Vector2D &pos, const gsl::Vector2D &p1,
                                                      const gsl::Vector2D &p2, const gsl::Vector2D &p3)
{
    gsl::Vector2D p12a = p2-p1;
    gsl::Vector2D p13a = p3-p1;
    gsl::Vector3D p12{p12a.x, p12a.y, 0};
    gsl::Vector3D p13{p13a.x, p13a.y, 0};
    gsl::Vector3D n = gsl::Vector3D::cross(p12, p13);
    float areal_123 = n.length(); // dobbelt areal
    gsl::Vector3D baryc;
    // u
    gsl::Vector2D p0 = p2 - pos;
    gsl::Vector2D q0 = p3 - pos;
    gsl::Vector3D p{p0.x, p0.y, 0};
    gsl::Vector3D q{q0.x, q0.y, 0};

    n = gsl::Vector3D::cross(p,q);
    baryc.setX(n.z/areal_123);
    // v
    p0 = p3 - pos;
    q0 = p1 - pos;
    p = gsl::Vector3D{p0.x, p0.y, 0};
    q = gsl::Vector3D{q0.x, q0.y, 0};

    n = gsl::Vector3D::cross(p,q);
    baryc.setY(n.z/areal_123);
    // w
    p0 = p1 - pos;
    q0 = p2 - pos;
    p = gsl::Vector3D{p0.x, p0.y, 0};
    q = gsl::Vector3D{q0.x, q0.y, 0};

    n = gsl::Vector3D::cross(p,q);
    baryc.setZ(n.z/areal_123);
    return baryc;
}
void ResourceManager::makeSphereRadius(Mesh *meshIn, gsl::Vector3D &vertexIn)
{
    //making correct bounding sphere radius:
    float length = vertexIn.length();
    if(length > meshIn->sphereRadius)
        meshIn->sphereRadius = length;
}


int ResourceManager::readObj(std::string filename) //Ole's obj reader code
{
    //should check if this object is new before this!
        mMeshComponents.emplace_back(Mesh());
        *object->mesh = mMeshComponents.back();


        std::string tempString = filename;
        filename.erase(filename.find(".obj"));

        for (unsigned short lod{0}; lod < 3; lod++ )
        {
            if(lod != 0)
                tempString = filename + std::to_string(lod) + ".obj";

            std::ifstream fileIn;
            fileIn.open (tempString, std::ifstream::in);
            if(!fileIn)
                qDebug() << "Could not open file for reading: " << QString::fromStdString(tempString);

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

                    if(lod == 0)
                        makeSphereRadius(object->mesh, tempVertex);

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
                            object->mesh->mVertices[lod].push_back(tempVert);
                        }
                        else            //no uv in mesh data, use 0, 0 as uv
                        {
                            Vertex tempVert(tempVertecies[index], tempNormals[normal], gsl::Vector2D(0.0f, 0.0f));
                            object->mesh->mVertices[lod].push_back(tempVert);
                        }
                        object->mesh->mIndices[lod].push_back(temp_index++);
                    }
                    continue;
                }
            }
            fileIn.close();

            init(*object->mesh, lod);
        }

        return mMeshComponents.size()-1;    //returns index to last object
}

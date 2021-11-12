#include "shapefactory.h"

Circle::Circle()
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMaterial = new MaterialComponent();
    mNameComp = new NameComponent();
}

Square::Square()
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMaterial = new MaterialComponent();
    mNameComp = new NameComponent();
}

Plain::Plain()
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMaterial = new MaterialComponent();
    mNameComp = new NameComponent();
}

BigWall::BigWall()
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMaterial = new MaterialComponent();
    mNameComp = new NameComponent();
}

SmallWall::SmallWall()
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMaterial = new MaterialComponent();
    mNameComp = new NameComponent();
}

//Enemy::Enemy()
//{
//    mTransform = new TransformComponent();
//    mTransform->mMatrix.setToIdentity();
//    mNameComp = new NameComponent();
//    mMaterial = new MaterialComponent();
//    //readFile("../GEA2021/Assets/Enemy.obj");
//}


ObjMesh::ObjMesh()
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mNameComp = new NameComponent();
    mMaterial = new MaterialComponent();
}

VisualObject* ShapeFactory::createShape(string shapeName)
{
    string obj{"obj"};
    string fileStr{"../GEA2021/Assets/" + shapeName};
    VisualObject* temp = nullptr;
    std::string a = to_string(mCounter-4); //For å legge til 1,2,3 osv bak navn på dupliserte objekter

    if (shapeName == "Circle")
    {
        temp = new Circle;
        temp->mMesh = myMeshes[0];
        temp->mTransform->mMatrix.scale(0.5);
        temp->mCollision = myCollis[0];
        temp->mCollision->setBoundingSphere(0.5, temp->mTransform->mPosition);

        if(doOnce[0] == false){
            temp->mNameComp->ObjectName = shapeName;
            doOnce[0] = true;}
        else
            temp->mNameComp->ObjectName = shapeName + a;

        temp->mNameComp->ObjectID = mCounter;
        mCounter++;

        return temp;
    }
    else if(shapeName == "Square")
    {
        temp = new Square;
        temp->mMesh = myMeshes[1];
        temp->mCollision = myCollis[1];
        temp->mCollision->setBoundingSphere(0.5, temp->mTransform->mPosition);

        if(doOnce[1] == false){
            temp->mNameComp->ObjectName = shapeName;
            doOnce[1] = true;}
        else
            temp->mNameComp->ObjectName = shapeName + a;

        temp->mNameComp->ObjectID = mCounter;
        mCounter++;

        return temp;
    }
    else if(shapeName == "Plain")
    {
        temp = new Plain;
        temp->mMesh = myMeshes[2];
        temp->mCollision = myCollis[2];
        temp->mCollision->setBoundingSphere(0.01, temp->mTransform->mPosition);

        if(doOnce[2] == false){
            temp->mNameComp->ObjectName = shapeName;
            doOnce[2] = true;}
        else
            temp->mNameComp->ObjectName = shapeName + a;

        temp->mNameComp->ObjectID = mCounter;
        mCounter++;

        return temp;
    }
//    else if(shapeName == "Enemy")
//    {
//        temp = new Enemy;
//        temp->mMesh = myMeshes[5];
//        temp->mCollision = myCollis[5];
//        temp->mTransform->mMatrix.scale(0.3);
//        temp->mCollision->setBoundingSphere(0.25, temp->mTransform->mPosition);

//        if(doOnce[3] == false){
//            temp->mNameComp->ObjectName = shapeName;
//            doOnce[3] = true;}
//        else
//            temp->mNameComp->ObjectName = shapeName + a;

//        temp->mNameComp->ObjectID = mCounter;
//        mCounter++;

//        return temp;
//    }
    //    else if(shapeName == "BigWall")
    //    {
    //        temp = new BigWall;
    //        temp->mMesh = myMeshes[3];
    //        temp->mCollision = myCollis[3];
    //        temp->mCollision->setBoundingSphere(0.25, temp->mTransform->mPosition);

    //        if(doOnce[3] == false){
    //            temp->mNameComp->mName = shapeName;
    //            doOnce[3] = true;}
    //        else
    //            temp->mNameComp->mName = shapeName + a;

    //        temp->mNameComp->objectID = mCounter;
    //        mCounter++;

    //        return temp;
    //    }
    //    else if(shapeName == "SmallWall")
    //    {
    //        temp = new SmallWall;
    //        temp->mMesh = myMeshes[4];
    //        temp->mCollision = myCollis[4];
    //        temp->mCollision->setBoundingSphere(0.25, temp->mTransform->mPosition);

    //        if(doOnce[4] == false){
    //            temp->mNameComp->mName = shapeName;
    //            doOnce[4] = true;}
    //        else
    //            temp->mNameComp->mName = shapeName + a;

    //        temp->mNameComp->objectID = mCounter;
    //        mCounter++;

    //        return temp;
    //    }
    else if(shapeName.find(obj) != std::string::npos)
    {
        temp = nullptr;
        if(myObjs.size()>=1)
        {
            for(auto om = myObjs.begin(); om != myObjs.end(); om++)
            {
                if (shapeName.find(om->first) != std::string::npos)
                {
                    temp = new ObjMesh();
                    temp->mMesh = myMeshes[om->second];
                    temp->mCollision = myCollis[om->second];
                    temp->mCollision->setBoundingSphere(temp->mCollision->radius, temp->mTransform->mPosition);
                    temp->mNameComp->ObjectName = om->first + "_" + a;
                    temp->mNameComp->ObjectID = mCounter;
                    mCounter++;
                }
            }
            if(temp!=nullptr)
                return temp;
            else{
                temp = new ObjMesh();
                temp->mMesh = myMeshes[mCounter];
                temp->mCollision = myCollis[mCounter];
                temp->mCollision->setBoundingSphere(temp->mCollision->radius, temp->mTransform->mPosition);
                shapeName.pop_back();shapeName.pop_back();shapeName.pop_back();shapeName.pop_back();
                myObjs[shapeName] = mCounter;
                temp->mNameComp->ObjectName = shapeName;
                temp->mNameComp->ObjectID = mCounter;
                mCounter++;
                return temp;}
        }
        else{
            temp = new ObjMesh();
            temp->mMesh = myMeshes[mCounter];
            temp->mCollision = myCollis[mCounter];
            temp->mCollision->setBoundingSphere(temp->mCollision->radius, temp->mTransform->mPosition);
            shapeName.pop_back();shapeName.pop_back();shapeName.pop_back();shapeName.pop_back();
            myObjs[shapeName] = mCounter;
            temp->mNameComp->ObjectName = shapeName;
            temp->mNameComp->ObjectID = mCounter;
            mCounter++;
            return temp;}
    }
    else{
        qDebug() << "createShape: invalid string";
        return nullptr;}
}

void ShapeFactory::makeVertices()
{
    MeshComponent* m = nullptr;
    CollisionComponent* c = nullptr;
    float x, y, z;
    int m_rekursjoner = 3;

    m = new MeshComponent;
    c = new CollisionComponent;
    m->mDrawType = GL_TRIANGLES;
    m->mVertices.reserve(3 * 8 * pow(4, m_rekursjoner));
    myMeshes.push_back(m);
    myCollis.push_back(c);

    float a = 1;
    gsl::Vector3D v0{0, 0, a};
    gsl::Vector3D v1{a, 0, 0};
    gsl::Vector3D v2{0, a, 0};
    gsl::Vector3D v3{-a, 0, 0};
    gsl::Vector3D v4{0, -a, 0};
    gsl::Vector3D v5{0, 0, -a};

    subDivide(v0, v1, v2, m_rekursjoner);
    subDivide(v0, v2, v3, m_rekursjoner);
    subDivide(v0, v3, v4, m_rekursjoner);
    subDivide(v0, v4, v1, m_rekursjoner);
    subDivide(v5, v2, v1, m_rekursjoner);
    subDivide(v5, v3, v2, m_rekursjoner);
    subDivide(v5, v4, v3, m_rekursjoner);
    subDivide(v5, v1, v4, m_rekursjoner);

    m = new MeshComponent;
    c = new CollisionComponent;

    x = 0.5;
    y = 0.5;
    z = 0.5;

    m->mDrawType = GL_TRIANGLES;

    m->mVertices.push_back(Vertex{-x,-y,-z,  1,0,0});
    m->mVertices.push_back(Vertex{ x,-y,-z,  1,0,0});       // bottom surface
    m->mVertices.push_back(Vertex{-x, y,-z,  1,0,0});

    m->mVertices.push_back(Vertex{ x,-y,-z,  1,0,0});
    m->mVertices.push_back(Vertex{ x, y,-z,  1,0,0});
    m->mVertices.push_back(Vertex{-x, y,-z,  1,0,0});

    m->mVertices.push_back(Vertex{ x,-y,-z,  1,0,1});
    m->mVertices.push_back(Vertex{ x, y,-z,  1,0,1});       // right surface
    m->mVertices.push_back(Vertex{ x, y, z,  1,0,1});

    m->mVertices.push_back(Vertex{ x,-y,-z,  1,0,1});
    m->mVertices.push_back(Vertex{ x, y, z,  1,0,1});
    m->mVertices.push_back(Vertex{ x,-y, z,  1,0,1});

    m->mVertices.push_back(Vertex{-x,-y, z,  1,1,0});
    m->mVertices.push_back(Vertex{-x, y, z,  1,1,0});       // top surface
    m->mVertices.push_back(Vertex{ x, y, z,  1,1,0});

    m->mVertices.push_back(Vertex{-x,-y, z,  1,1,0});
    m->mVertices.push_back(Vertex{ x,-y, z,  1,1,0});
    m->mVertices.push_back(Vertex{ x, y, z,  1,1,0});

    m->mVertices.push_back(Vertex{-x,-y, z,  1,0,0});
    m->mVertices.push_back(Vertex{-x,-y,-z,  1,0,0});       //left surface
    m->mVertices.push_back(Vertex{-x, y, z,  1,0,0});

    m->mVertices.push_back(Vertex{-x,-y,-z,  1,0,0});
    m->mVertices.push_back(Vertex{-x, y,-z,  1,0,0});
    m->mVertices.push_back(Vertex{-x, y, z,  1,0,0});

    m->mVertices.push_back(Vertex{-x, y,-z,  1,0,1});
    m->mVertices.push_back(Vertex{-x, y, z,  1,0,1});   // back surface
    m->mVertices.push_back(Vertex{ x, y,-z,  1,0,1});

    m->mVertices.push_back(Vertex{ x, y,-z,  1,0,1});
    m->mVertices.push_back(Vertex{ x, y, z,  1,0,1});
    m->mVertices.push_back(Vertex{-x, y, z,  1,0,1});

    gsl::Vector3D min(-x,-y,-z);
    gsl::Vector3D max( x, y, z);

    c->setBoundingBox(min, max);

    myMeshes.push_back(m);
    myCollis.push_back(c);

    m = new MeshComponent;
    c = new CollisionComponent;

    m->mDrawType = GL_TRIANGLES;

    //24 x 18 fra -12 til +12 -9 til +9
    x = 5;
    z = 5;
    m->mVertices.push_back(Vertex{-x, 0.0,-z,  0.0, 0.3, 0.2});
    m->mVertices.push_back(Vertex{-x, 0.0, z,  0.0, 0.3, 0.2});
    m->mVertices.push_back(Vertex{ x, 0.0,-z,  0.0, 0.3, 0.2});

    m->mVertices.push_back(Vertex{ x, 0.0, z,  0.0, 0.3, 0.2});
    m->mVertices.push_back(Vertex{-x, 0.0, z, 0.0, 0.3, 0.2});
    m->mVertices.push_back(Vertex{ x, 0.0,-z,  0.0, 0.3, 0.2});

    myMeshes.push_back(m);
    myCollis.push_back(c);

//    m = new MeshComponent;
//    c = new CollisionComponent;

//    m->mDrawType = GL_TRIANGLES;

//    x = 4.25;
//    y = 0.75;
//    z = 0.25;

//    m->mVertices.push_back(Vertex{-x,-y,-z,    1,0,0});
//    m->mVertices.push_back(Vertex{ x,-y,-z,    1,0,0});       // bottom surface
//    m->mVertices.push_back(Vertex{-x, y,-z,    1,0,0});

//    m->mVertices.push_back(Vertex{ x,-y,-z,    1,0,0});
//    m->mVertices.push_back(Vertex{ x, y,-z,    1,0,0});
//    m->mVertices.push_back(Vertex{-x, y,-z,    1,0,0});

//    m->mVertices.push_back(Vertex{ x,-y,-z,    1,0,1});
//    m->mVertices.push_back(Vertex{ x, y,-z,    1,0,1});       // right surface
//    m->mVertices.push_back(Vertex{ x, y, z,    1,0,1});

//    m->mVertices.push_back(Vertex{ x,-y,-z,    1,0,1});
//    m->mVertices.push_back(Vertex{ x, y, z,    1,0,1});
//    m->mVertices.push_back(Vertex{ x,-y, z,    1,0,1});

//    m->mVertices.push_back(Vertex{-x,-y, z,    1,1,0});
//    m->mVertices.push_back(Vertex{-x, y, z,    1,1,0});       // top surface
//    m->mVertices.push_back(Vertex{ x, y, z,    1,1,0});

//    m->mVertices.push_back(Vertex{-x,-y, z,    1,1,0});
//    m->mVertices.push_back(Vertex{ x,-y, z,    1,1,0});
//    m->mVertices.push_back(Vertex{ x, y, z,    1,1,0});

//    m->mVertices.push_back(Vertex{-x,-y, z,    1,0,0});
//    m->mVertices.push_back(Vertex{-x,-y,-z,    1,0,0});       //left surface
//    m->mVertices.push_back(Vertex{-x, y, z,    1,0,0});

//    m->mVertices.push_back(Vertex{-x,-y,-z,    1,0,0});
//    m->mVertices.push_back(Vertex{-x, y,-z,    1,0,0});
//    m->mVertices.push_back(Vertex{-x, y, z,    1,0,0});

//    m->mVertices.push_back(Vertex{-x, y,-z,    1,0,1});
//    m->mVertices.push_back(Vertex{-x, y, z,    1,0,1});   // back surface
//    m->mVertices.push_back(Vertex{ x, y,-z,    1,0,1});

//    m->mVertices.push_back(Vertex{ x, y,-z,    1,0,1});
//    m->mVertices.push_back(Vertex{ x, y, z,    1,0,1});
//    m->mVertices.push_back(Vertex{-x, y, z,    1,0,1});


//    min = gsl::Vector3D(-x,-y,-z);
//    max = gsl::Vector3D( x, y, z);

//    c->setBoundingBox(min, max);

    //myMeshes.push_back(m);
    //myCollis.push_back(c);

//    m = new MeshComponent;
//    c = new CollisionComponent;

//    x = 2.25;
//    y = 0.75;
//    z = 0.25;

//    m->mDrawType = GL_TRIANGLES;

//    m->mVertices.push_back(Vertex{-x,-y,-z,   1,0,0});
//    m->mVertices.push_back(Vertex{ x,-y,-z,   1,0,0});       // bottom surface
//    m->mVertices.push_back(Vertex{-x, y,-z,   1,0,0});

//    m->mVertices.push_back(Vertex{ x,-y,-z,   1,0,0});
//    m->mVertices.push_back(Vertex{ x, y,-z,   1,0,0});
//    m->mVertices.push_back(Vertex{-x, y,-z,   1,0,0});

//    m->mVertices.push_back(Vertex{ x,-y,-z,   1,0,1});
//    m->mVertices.push_back(Vertex{ x, y,-z,   1,0,1});       // right surface
//    m->mVertices.push_back(Vertex{ x, y, z,   1,0,1});

//    m->mVertices.push_back(Vertex{ x,-y,-z,   1,0,1});
//    m->mVertices.push_back(Vertex{ x, y, z,   1,0,1});
//    m->mVertices.push_back(Vertex{ x,-y, z,   1,0,1});

//    m->mVertices.push_back(Vertex{-x,-y, z,   1,1,0});
//    m->mVertices.push_back(Vertex{-x, y, z,   1,1,0});       // top surface
//    m->mVertices.push_back(Vertex{ x, y, z,   1,1,0});

//    m->mVertices.push_back(Vertex{-x,-y, z,   1,1,0});
//    m->mVertices.push_back(Vertex{ x,-y, z,   1,1,0});
//    m->mVertices.push_back(Vertex{ x, y, z,   1,1,0});

//    m->mVertices.push_back(Vertex{-x,-y, z,   1,0,0});
//    m->mVertices.push_back(Vertex{-x,-y,-z,   1,0,0});       //left surface
//    m->mVertices.push_back(Vertex{-x, y, z,   1,0,0});

//    m->mVertices.push_back(Vertex{-x,-y,-z,   1,0,0});
//    m->mVertices.push_back(Vertex{-x, y,-z,   1,0,0});
//    m->mVertices.push_back(Vertex{-x, y, z,   1,0,0});

//    m->mVertices.push_back(Vertex{-x, y,-z,   1,0,1});
//    m->mVertices.push_back(Vertex{-x, y, z,   1,0,1});   // back surface
//    m->mVertices.push_back(Vertex{ x, y,-z,   1,0,1});

//    m->mVertices.push_back(Vertex{ x, y,-z,   1,0,1});
//    m->mVertices.push_back(Vertex{ x, y, z,   1,0,1});
//    m->mVertices.push_back(Vertex{-x, y, z,   1,0,1});

//    min = gsl::Vector3D(-x,-y,-z);
//    max = gsl::Vector3D( x, y, z);

//    c->setBoundingBox(min, max);

    //myMeshes.push_back(m);
    //myCollis.push_back(c);

    m = new MeshComponent;
    c = new CollisionComponent;

    readFile(monkeyString, m);
    m->mDrawType = GL_TRIANGLES;
    c->radius = 0.2;

    myMeshes.push_back(m);
    myCollis.push_back(c);

    m = new MeshComponent;
    c = new CollisionComponent;

    readFile(pacmanString, m);
    m->mDrawType = GL_TRIANGLES;
    c->radius = 0.5;

    myMeshes.push_back(m);
    myCollis.push_back(c);

    m = new MeshComponent;
    c = new CollisionComponent;

    readFile(enemyString, m);
    m->mDrawType = GL_TRIANGLES;
    c->radius = 0.5;

    myMeshes.push_back(m);
    myCollis.push_back(c);
}

MeshComponent *ShapeFactory::getMesh(int i)
{
    return myMeshes[i];
}

CollisionComponent *ShapeFactory::getColli(int i)
{
    return myCollis[i];
}

void ShapeFactory::readFile(std::string filename, MeshComponent* m)
{
    //Open File
    //    std::string filename = Orf::assetFilePath.toStdString() + fileName + ".obj";
    std::ifstream fileIn;
    fileIn.open (filename, std::ifstream::in);
    if(!fileIn)
        qDebug() << "Could not open file for reading: " << QString::fromStdString(filename);

    //One line at a time-variable
    std::string oneLine;
    //One word at a time-variable
    std::string oneWord;

    std::vector <gsl::Vector3D> tempVertecies;
    std::vector <gsl::Vector3D> tempNormals;
    std::vector <gsl::Vector2D> tempUVs;

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
            tempVertex.setX (std::stof(oneWord));
            sStream >> oneWord;
            tempVertex.setY(std::stof(oneWord));
            sStream >> oneWord;
            tempVertex.setZ  (std::stof(oneWord));

            //Vertex made - pushing it into vertex-vector

            tempVertecies.push_back(tempVertex);

            continue;
        }
        if (oneWord == "vt")
        {
            //            qDebug() << "Line is UV-coordinate "  << QString::fromStdString(oneWord) << " ";
            gsl::Vector2D tempUV;
            sStream >> oneWord;
            tempUV.setX  (std::stof(oneWord));
            sStream >> oneWord;
            tempUV.setY  (std::stof(oneWord));

            //UV made - pushing it into UV-vector
            tempUVs.push_back(tempUV);

            continue;
        }
        if (oneWord == "vn")
        {
            //            qDebug() << "Line is normal "  << QString::fromStdString(oneWord) << " ";
            gsl::Vector3D tempNormal;
            sStream >> oneWord;
            tempNormal.setX (std::stof(oneWord));
            sStream >> oneWord;
            tempNormal.setY  (std::stof(oneWord));
            sStream >> oneWord;
            tempNormal.setZ (std::stof(oneWord));

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
                    m->mVertices.push_back(tempVert);
                }
                else            //no uv in mesh data, use 0, 0 as uv
                {
                    Vertex tempVert(tempVertecies[index], tempNormals[normal], gsl::Vector2D(0.0f, 0.0f));
                    m->mVertices.push_back(tempVert);
                }
                m->mIndices.push_back(temp_index++);
            }
            continue;
        }
    }
    fileIn.close();
}

void ShapeFactory::subDivide(const gsl::Vector3D &a, const gsl::Vector3D &b, const gsl::Vector3D &c, int n)
{
    if(n>0)
    {
        gsl::Vector3D v1 = a+b; v1.normalize();
        gsl::Vector3D v2 = a+c; v2.normalize();
        gsl::Vector3D v3 = c+b; v3.normalize();
        subDivide(a, v1, v2, n-1);
        subDivide(c, v2, v3, n-1);
        subDivide(b, v3, v1, n-1);
        subDivide(v3, v2, v1, n-1);
    }else{
        Vertex v{a.x, a.y, a.z, a.x, a.y, a.z};
        myMeshes[0]->mVertices.push_back(v);
        v = Vertex{b.x, b.y, b.z, b.x, b.y, b.z};
        myMeshes[0]->mVertices.push_back(v);
        v = Vertex{c.x, c.y, c.z, c.x, c.y, c.z};
        myMeshes[0]->mVertices.push_back(v);
    }
}

#include "shapefactory.h"
#include "component.h"
#include <cmath>
#include <math.h>
#include "gltypes.h"


Plain::Plain()
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();

    mMesh = new MeshComponent();


    mMesh->mVertices.push_back(Vertex{-4.0, 0.0,-3.0,  0,0,1,0,0});
    mMesh->mVertices.push_back(Vertex{-4.0, 0.0, 3.0,  0,0,1,0,0}); //floor
    mMesh->mVertices.push_back(Vertex{4.0,  0.0,-3.0,  0,0,1,0,0});
    mMesh->mVertices.push_back(Vertex{4.0,  0.0, 3.0,  0,0,1,0,0});
    mMesh->mVertices.push_back(Vertex{-4.0, 0.0, 3.0,  0,0,1,0,0});
    mMesh->mVertices.push_back(Vertex{4.0,  0.0,-3.0,  0,0,1,0,0});

    mMesh->mDrawType = GL_TRIANGLES;

    mMaterial = new MaterialComponent();
}

Triangle::Triangle()
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMesh = new MeshComponent();
    mMesh->mDrawType = GL_TRIANGLES;
    mMaterial = new MaterialComponent();

    // Positions // Color //UV
    mMesh->mVertices.push_back(Vertex{-0.9,0.0,0.0, 0.0,1.0,0.0});
    mMesh->mVertices.push_back(Vertex{-0.1,0.0,0.0, 1.0,0.0,0.0});
    mMesh->mVertices.push_back(Vertex{-0.5,0.6532,0.0, 0.0,0.0,1.0});

    mMesh->mVertices.push_back(Vertex{-0.5,0.6532,0.0, 0.0,0.0,1.0});
    mMesh->mVertices.push_back(Vertex{-0.9,0.0,0.0, 0.0,1.0,0.0});
    mMesh->mVertices.push_back(Vertex{-0.5,0.3266,0.6532, 1.0,0.0,0.0});

    mMesh->mVertices.push_back(Vertex{-0.5,0.6532,0.0, 0.0,0.0,1.0});
    mMesh->mVertices.push_back(Vertex{-0.1,0.0,0.0, 0.0,1.0,0.0});
    mMesh->mVertices.push_back(Vertex{-0.5,0.3266,0.6532, 1.0,0.0,0.0});

    mMesh->mVertices.push_back(Vertex{-0.9,0.0,0.0, 0.0,1.0,0.0});
    mMesh->mVertices.push_back(Vertex{-0.1,0.0,0.0, 1.0,0.0,0.0});
    mMesh->mVertices.push_back(Vertex{-0.5,0.3266,0.6532, 0.0,0.0,1.0});


}

Square::Square()
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMesh = new MeshComponent();
    mMesh->mDrawType = GL_TRIANGLES;
    mMaterial = new MaterialComponent();

    mMesh->mVertices.push_back(Vertex{0,0,0, 1,0,0});
    mMesh->mVertices.push_back(Vertex{0.5,0,0, 1,0,0});       // bottom surface
    mMesh->mVertices.push_back(Vertex{0,0.5,0, 1,0,0});

    mMesh->mVertices.push_back(Vertex{0.5,0,0,  1,0,0});
    mMesh->mVertices.push_back(Vertex{0.5,0.5,0,  1,0,0});
    mMesh->mVertices.push_back(Vertex{0,0.5,0,  1,0,0});

    mMesh->mVertices.push_back(Vertex{0.5,0,0,  1,0,1});
    mMesh->mVertices.push_back(Vertex{0.5,0.5,0,  1,0,1});       // right surface
    mMesh->mVertices.push_back(Vertex{0.5,0.5,0.5,  1,0,1});

    mMesh->mVertices.push_back(Vertex{0.5,0,0,  1,0,1});
    mMesh->mVertices.push_back(Vertex{0.5,0.5,0.5,  1,0,1});
    mMesh->mVertices.push_back(Vertex{0.5,0,0.5,  1,0,1});

    mMesh->mVertices.push_back(Vertex{0,0,0.5,  1,1,0});
    mMesh->mVertices.push_back(Vertex{0,0.5,0.5,  1,1,0});       // top surface
    mMesh->mVertices.push_back(Vertex{0.5,0.5,0.5,  1,1,0});
    mMesh->mVertices.push_back(Vertex{0,0,0.5,  1,1,0});
    mMesh->mVertices.push_back(Vertex{0.5,0,0.5,  1,1,0});
    mMesh->mVertices.push_back(Vertex{0.5,0.5,0.5,  1,1,0});

    mMesh->mVertices.push_back(Vertex{0,0,0.5,  1,0,0});
    mMesh->mVertices.push_back(Vertex{0,0,0,  1,0,0});       //left surface
    mMesh->mVertices.push_back(Vertex{0,0.5,0.5,  1,0,0});

    mMesh->mVertices.push_back(Vertex{0,0,0,  1,0,0});
    mMesh->mVertices.push_back(Vertex{0,0.5,0,  1,0,0});
    mMesh->mVertices.push_back(Vertex{0,0.5,0.5,  1,0,0});

    mMesh->mVertices.push_back(Vertex{0,0.5,0,  1,0,1});
    mMesh->mVertices.push_back(Vertex{0,0.5,0.5,  1,0,1});   // back surface
    mMesh->mVertices.push_back(Vertex{0.5,0.5,0,  1,0,1});

    mMesh->mVertices.push_back(Vertex{0.5,0.5,0,  1,0,1});
    mMesh->mVertices.push_back(Vertex{0.5,0.5,0.5,  1,0,1});
    mMesh->mVertices.push_back(Vertex{0,0.5,0.5,  1,0,1});
}
//--------------------------Sphere-----------------------------------
Circle::Circle()
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMesh = new MeshComponent();
    mMesh->mDrawType = GL_TRIANGLES;
    mMaterial = new MaterialComponent();
    mMesh->mVertices.reserve(3 * 8 * pow(4, m_rekursjoner));
    circleUnit();
}

void Circle::subDivide(const gsl::Vector3D &a, const gsl::Vector3D &b, const gsl::Vector3D &c, int n)
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
        makeTriangle(a, b, c);
    }
}

void Circle::circleUnit()
{
    gsl::Vector3D v0{0, 0, 1};
    gsl::Vector3D v1{1, 0, 0};
    gsl::Vector3D v2{0, 1, 0};
    gsl::Vector3D v3{-1, 0, 0};
    gsl::Vector3D v4{0, -1, 0};
    gsl::Vector3D v5{0, 0, -1};

    subDivide(v0, v1, v2, m_rekursjoner);
    subDivide(v0, v2, v3, m_rekursjoner);
    subDivide(v0, v3, v4, m_rekursjoner);
    subDivide(v0, v4, v1, m_rekursjoner);
    subDivide(v5, v2, v1, m_rekursjoner);
    subDivide(v5, v3, v2, m_rekursjoner);
    subDivide(v5, v4, v3, m_rekursjoner);
    subDivide(v5, v1, v4, m_rekursjoner);
}

void Circle::makeTriangle(const gsl::Vector3D &v1, const gsl::Vector3D &v2, const gsl::Vector3D &v3)
{
    Vertex v{v1.x, v1.y, v1.z, v1.x, v1.y, v1.z};
    mMesh->mVertices.push_back(v);
    v = Vertex{v2.x, v2.y, v2.z, v2.x, v2.y, v2.z};
    mMesh->mVertices.push_back(v);
    v = Vertex{v3.x, v3.y, v3.z, v3.x, v3.y, v3.z};
    mMesh->mVertices.push_back(v);

}

//--------------------------OBJ-----------------------------------

Obj::Obj(std::string filename)
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();

    mMesh = new MeshComponent();

    readFile(filename);

    mMesh->mDrawType = GL_TRIANGLES;

    mMaterial = new MaterialComponent();

}

void Obj::readFile(std::string filename)
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
            continue;
        }
        if (oneWord == "")
        {
            continue;
        }
        if (oneWord == "v")
        {
            gsl::Vector3D tempVertex;
            sStream >> oneWord;
            tempVertex.setX (std::stof(oneWord));
            sStream >> oneWord;
            tempVertex.setY(std::stof(oneWord));
            sStream >> oneWord;
            tempVertex.setZ  (std::stof(oneWord));

            tempVertecies.push_back(tempVertex);

            continue;
        }
        if (oneWord == "vt")
        {
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
                    mMesh->mVertices.push_back(tempVert);
                }
                else            //no uv in mesh data, use 0, 0 as uv
                {
                    Vertex tempVert(tempVertecies[index], tempNormals[normal], gsl::Vector2D(0.0f, 0.0f));
                    mMesh->mVertices.push_back(tempVert);
                }
                mMesh->mIndices.push_back(temp_index++);
            }
            continue;
        }
    }
    fileIn.close();
}



VisualObject* ShapeFactory::createShape(string shapeName)
{
    if (shapeName == "Circle"){
        createCircle();
        return myShapes[0];}
    else if(shapeName == "Square"){
        createSquare();
        return myShapes[1];}
    else if(shapeName == "Triangle"){
        createTriangle();
        return myShapes[2];}
    else if(shapeName == "Plain"){
        createPlain();
        return myShapes[3];}
    else if(shapeName == "Obj"){
        createObj();
        return myShapes[4];}
    else{
        std::cout << "invalid string" << std::endl;
        return nullptr;}
}

void ShapeFactory::createCircle()
{
    myShapes[0] = new Circle;
}

void ShapeFactory::createSquare()
{
    myShapes[1] = new Square;
}

void ShapeFactory::createTriangle()
{
    myShapes[2] = new Triangle;
}

void ShapeFactory::createPlain()
{
    myShapes[3] = new Plain;
}

void ShapeFactory::createObj()
{
    myShapes[4] = new Obj("../GEA2021/Assets/Monkey.obj");
}




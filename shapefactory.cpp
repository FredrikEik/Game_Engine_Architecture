#include "shapefactory.h"

Circle::Circle()
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMesh = new MeshComponent();
    makeVerticies();
    mMaterial = new MaterialComponent();
    mNameComp = new NameComponent();
}

void Circle::makeVerticies()
{
    mMesh->mDrawType = GL_TRIANGLES;
    mMesh->mVertices.reserve(3 * 8 * pow(4, m_rekursjoner));
    mCollision = new CollisionComponent;
    circleUnit(mCollision);
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

void Circle::circleUnit(CollisionComponent* dCollision)
{
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

    dCollision->setBoundingSphere(a/2, mTransform->mPosition);
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

Triangle::Triangle()
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMesh = new MeshComponent();
    mCollision = new CollisionComponent;
    makeVerticies();
    mMaterial = new MaterialComponent();
    mNameComp = new NameComponent();
}

void Triangle::makeVerticies()
{
    mMesh->mDrawType = GL_TRIANGLES;

    // Positions            // Colors       //UV
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

    gsl::Vector3D min(-0.9, 0,0);
    gsl::Vector3D max(-0.1,0.6532, 0.6532);

    mCollision->setBoundingBox(min, max);
}

Square::Square()
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMesh = new MeshComponent();
    mCollision = new CollisionComponent;
    makeVerticies(mMesh, mCollision);
    mMaterial = new MaterialComponent();
    mNameComp = new NameComponent();
}

void Square::makeVerticies(MeshComponent* dMesh, CollisionComponent* dCollision)
{
    dMesh->mDrawType = GL_TRIANGLES;

    dMesh->mVertices.push_back(Vertex{0,0,0, 1,0,0});
    dMesh->mVertices.push_back(Vertex{0.5,0,0, 1,0,0});       // bottom surface
    dMesh->mVertices.push_back(Vertex{0,0.5,0, 1,0,0});

    dMesh->mVertices.push_back(Vertex{0.5,0,0,  1,0,0});
    dMesh->mVertices.push_back(Vertex{0.5,0.5,0,  1,0,0});
    dMesh->mVertices.push_back(Vertex{0,0.5,0,  1,0,0});

    dMesh->mVertices.push_back(Vertex{0.5,0,0,  1,0,1});
    dMesh->mVertices.push_back(Vertex{0.5,0.5,0,  1,0,1});       // right surface
    dMesh->mVertices.push_back(Vertex{0.5,0.5,0.5,  1,0,1});

    dMesh->mVertices.push_back(Vertex{0.5,0,0,  1,0,1});
    dMesh->mVertices.push_back(Vertex{0.5,0.5,0.5,  1,0,1});
    dMesh->mVertices.push_back(Vertex{0.5,0,0.5,  1,0,1});

    dMesh->mVertices.push_back(Vertex{0,0,0.5,  1,1,0});
    dMesh->mVertices.push_back(Vertex{0,0.5,0.5,  1,1,0});       // top surface
    dMesh->mVertices.push_back(Vertex{0.5,0.5,0.5,  1,1,0});

    dMesh->mVertices.push_back(Vertex{0,0,0.5,  1,1,0});
    dMesh->mVertices.push_back(Vertex{0.5,0,0.5,  1,1,0});
    dMesh->mVertices.push_back(Vertex{0.5,0.5,0.5,  1,1,0});

    dMesh->mVertices.push_back(Vertex{0,0,0.5,  1,0,0});
    dMesh->mVertices.push_back(Vertex{0,0,0,  1,0,0});       //left surface
    dMesh->mVertices.push_back(Vertex{0,0.5,0.5,  1,0,0});

    dMesh->mVertices.push_back(Vertex{0,0,0,  1,0,0});
    dMesh->mVertices.push_back(Vertex{0,0.5,0,  1,0,0});
    dMesh->mVertices.push_back(Vertex{0,0.5,0.5,  1,0,0});

    dMesh->mVertices.push_back(Vertex{0,0.5,0,  1,0,1});
    dMesh->mVertices.push_back(Vertex{0,0.5,0.5,  1,0,1});   // back surface
    dMesh->mVertices.push_back(Vertex{0.5,0.5,0,  1,0,1});

    dMesh->mVertices.push_back(Vertex{0.5,0.5,0,  1,0,1});
    dMesh->mVertices.push_back(Vertex{0.5,0.5,0.5,  1,0,1});
    dMesh->mVertices.push_back(Vertex{0,0.5,0.5,  1,0,1});

    gsl::Vector3D min(0,0,0);
    gsl::Vector3D max(0.5, 0.5, 0.5);

    dCollision->setBoundingBox(min, max);
}

Plain::Plain()
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMesh = new MeshComponent();
    makeVerticies(mMesh);
    mMaterial = new MaterialComponent();
    mNameComp = new NameComponent();
}

void Plain::makeVerticies(MeshComponent* dMesh)
{
    dMesh->mDrawType = GL_TRIANGLES;

    //24 x 18 fra -12 til +12 -9 til +9
    mTransform->scale = 3;
    float a = 4 * mTransform->scale;
    float b = 3 * mTransform->scale;
    dMesh->mVertices.push_back(Vertex{-a, 0.0,-b,  0.0, 0.3, 0.2});
    dMesh->mVertices.push_back(Vertex{-a, 0.0, b,  0.0, 0.3, 0.2});
    dMesh->mVertices.push_back(Vertex{a,  0.0,-b,  0.0, 0.3, 0.2});

    dMesh->mVertices.push_back(Vertex{a,  0.0, b,  0.0, 0.3, 0.2});
    dMesh->mVertices.push_back(Vertex{-a, 0.0, b, 0.0, 0.3, 0.2});
    dMesh->mVertices.push_back(Vertex{a,  0.0,-b,  0.0, 0.3, 0.2});
}

ObjMesh::ObjMesh(std::string filename)
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMesh = new MeshComponent();
    mCollision = new CollisionComponent;
    mNameComp = new NameComponent();

    readFile(filename);

    mMesh->mDrawType = GL_TRIANGLES;
    mMaterial = new MaterialComponent();

}

VisualObject* ShapeFactory::createShape(string shapeName)
{
    string obj{"obj"};
    if (shapeName == "Circle"){
        if(myShapes[0]->mNameComp->mName == shapeName){
            return myShapes[0];}
        else{
            myShapes[0] = new Circle;
            myShapes[0]->mNameComp->mName = shapeName;
            myShapes[0]->mNameComp->objectID = 0;
            return myShapes[0];}}
    else if(shapeName == "Square"){
        if(myShapes[1]->mNameComp->mName == shapeName)
            return myShapes[1];
        else{
            myShapes[1] = new Square;
            myShapes[1]->mNameComp->mName = shapeName;
            myShapes[1]->mNameComp->objectID = 1;
            return myShapes[1];}}
    else if(shapeName == "Triangle"){
        if(myShapes[2]->mNameComp->mName == shapeName)
            return myShapes[2];
        else{
            myShapes[2] = new Triangle();
            myShapes[2]->mNameComp->mName = shapeName;
            myShapes[2]->mNameComp->objectID = 2;
            return myShapes[2];}}
    else if(shapeName == "Plain"){
        if(myShapes[3]->mNameComp->mName == shapeName)
            return myShapes[3];
        else{
            myShapes[3] = new Plain();
            myShapes[3]->mNameComp->mName = shapeName;
            myShapes[3]->mNameComp->objectID = 3;
            return myShapes[3];}}
    else if(shapeName.find(obj) != std::string::npos)
    {
        VisualObject* temp = nullptr;
        if(myObjs.size()>=1)
        {
            for(auto a = myObjs.begin(); a != myObjs.end(); a++)
            {
                if (shapeName.find(a->first) != std::string::npos)
                    temp = myShapes[a->second];

            }
            return temp;
        }
        else{
            myShapes[ObjStartID] = (new ObjMesh("../GEA2021/Assets/" + shapeName));
            shapeName.pop_back();shapeName.pop_back();shapeName.pop_back();shapeName.pop_back();
            myObjs[shapeName] = ObjStartID;
            return myShapes[ObjStartID];
            ObjStartID++;}
    }
    else{
        std::cout << "invalid string" << std::endl;
        return nullptr;}
}



//VisualObject* ShapeFactory::createMonkey()
//{
//    if(doOnce == false)
//    {
//        myMonkey = new ObjMesh(monkeyString);
//        return myMonkey;
//        doOnce = true;
//    }
//    else
//        return myMonkey;
//}

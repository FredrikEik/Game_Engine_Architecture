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




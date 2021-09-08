#include "shapefactory.h"


Shape::Shape()
{
    //    mTransform = new TransformComponent();
    //    mTransform->mMatrix.setToIdentity();

    //    mMesh = new MeshComponent();
    //    // Positions            // Colors       //UV
    ////    mMesh->mVertices.push_back(Vertex{-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  0.f, 0.f}); // Bottom Left
    ////    mMesh->mVertices.push_back(Vertex{0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.f}); // Bottom Right
    ////    mMesh->mVertices.push_back(Vertex{0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.f}); // Top

    //    mMesh->mDrawType = GL_TRIANGLES;

    //    mMaterial = new MaterialComponent();
}

void Shape::init()
{
    //must call this to use OpenGL functions?
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mMesh->mVAO );
    glBindVertexArray( mMesh->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mMesh->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mMesh->mVBO );

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, mMesh->mVertices.size()*sizeof( Vertex ),
                  mMesh->mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0  );          // array buffer offset
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

Circle::Circle()
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();

    mMesh = new MeshComponent();
    // Positions            // Colors       //UV
    mMesh->mVertices.push_back(Vertex{-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  0.f, 0.f}); // Bottom Left
    mMesh->mVertices.push_back(Vertex{0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.f}); // Bottom Right
    mMesh->mVertices.push_back(Vertex{0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.f}); // Top

    mMesh->mDrawType = GL_TRIANGLES;

    mMaterial = new MaterialComponent();
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

void Circle::circleUnitBall()
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

Triangle::Triangle()
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();

    mMesh = new MeshComponent();
    // Positions            // Colors       //UV
    mMesh->mVertices.push_back(Vertex{-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  0.f, 0.f}); // Bottom Left
    mMesh->mVertices.push_back(Vertex{0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.f}); // Bottom Right
    mMesh->mVertices.push_back(Vertex{0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.f}); // Top

    mMesh->mDrawType = GL_TRIANGLES;

    mMaterial = new MaterialComponent();
}

Square::Square(int n)
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

void ShapeFactory::createShape(int id)
{
    if (id == 0)
    {
        if(circleExist==false)
        {
            myShapes[0] = new Circle;
            circleExist = true;
        }
        else if(circleExist == true)
        {
            delete myShapes[0];
            circleExist = false;
        }
    }
    else if(id == 1)
    {
        if(squareExist == false)
        {
            myShapes[1] = new Square;
            squareExist = true;
        }
        else if(squareExist == true)
        {
            delete myShapes[1];
            squareExist = false;
        }
    }
    else if(id == 2)
    {
        if(triangleExist == false)
        {
            myShapes[2] = new Triangle;
            triangleExist = true;
        }
        else if(triangleExist == true)
        {
            delete myShapes[2];
            triangleExist = false;
        }
    }
    else
        std::cout << "invalid id" << std::endl;
}

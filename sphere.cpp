//#include <iostream>
//#include <sstream>
//#include "sphere.h"
//#include "math.h"
//Sphere::Sphere()
//{

//}

//Sphere::Sphere(int n) : m_recursions{n}, m_index{0}, GameObject()
//{
//    mesh->mVertices.reserve(3*8*pow(4, m_recursions));
//    SphereSetup();
//}

//void Sphere::init()
//{
//    initializeOpenGLFunctions();

//    glGenVertexArrays( 1, &mesh->mVAO );
//    glBindVertexArray( mesh->mVAO );

//    glGenBuffers( 1, &mesh->mVBO );
//    glBindBuffer( GL_ARRAY_BUFFER, mesh->mVBO );

//    glBufferData( GL_ARRAY_BUFFER, mesh->mVertices.size()*sizeof( Vertex ), mesh->mVertices.data(), GL_STATIC_DRAW );

//    // 1st attribute buffer : vertices
//    glBindBuffer(GL_ARRAY_BUFFER, mesh->mVBO);
//    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
//    glEnableVertexAttribArray(0);

//    // 2nd attribute buffer : colors
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
//    glEnableVertexAttribArray(1);

//    // 3rd attribute buffer : uvs
//    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
//    glEnableVertexAttribArray(2);

//    glBindVertexArray(0);
//}

//void Sphere::draw()
//{
//    glBindVertexArray( mesh->mVAO );
//    glDrawArrays(GL_TRIANGLES, 0, 3);
//    glBindVertexArray(0);
//}

//void Sphere::SphereSetup()
//{
//    QVector3D v0{0, 0, 1};
//    QVector3D v1{1, 0, 0};
//    QVector3D v2{0, 1, 0};
//    QVector3D v3{-1, 0, 0};
//    QVector3D v4{0, -1, 0};
//    QVector3D v5{0, 0, -1};

//    subDivide(v0, v1, v2, m_recursions);
//    subDivide(v0, v2, v3, m_recursions);
//    subDivide(v0, v3, v4, m_recursions);
//    subDivide(v0, v4, v1, m_recursions);
//    subDivide(v5, v2, v1, m_recursions);
//    subDivide(v5, v3, v2, m_recursions);
//    subDivide(v5, v4, v3, m_recursions);
//    subDivide(v5, v1, v4, m_recursions);
//}

//void Sphere::makeTriangle(const QVector3D &v1, const QVector3D &v2, const QVector3D &v3)
//{
//    QVector3D vec1(v1 - v3);
//    QVector3D vec2(v1 - v2);
//    QVector3D n = -QVector3D::crossProduct(vec1, vec2);
//    n.normalize();

//    Vertex v{v1.x(), v1.y(), v1.z(), n.x(), n.y(), n.z()};
//    mesh->mVertices.push_back(v);
//    v = Vertex{v2.x(), v2.y(), v2.z(), n.x(), n.y(), n.z()};
//    mesh->mVertices.push_back(v);
//    v = Vertex{v3.x(), v3.y(), v3.z(), n.x(), n.y(), n.z()};
//    mesh->mVertices.push_back(v);
//}

//void Sphere::subDivide(const QVector3D &a, const QVector3D &b, const QVector3D c, int n)
//{
//    if (n>0) {
//        QVector3D v1 = a+b; v1.normalize();
//        QVector3D v2 = a+c; v2.normalize();
//        QVector3D v3 = c+b; v3.normalize();
//        subDivide(a, v1, v2, n-1);
//        subDivide(c, v2, v3, n-1);
//        subDivide(b, v3, v1, n-1);
//        subDivide(v3, v2, v1, n-1);
//    } else{
//        makeTriangle(a, b, c);
//    }
//}

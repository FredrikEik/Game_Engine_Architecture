#include "octahedronball.h"
#include "vertex.h"

OctahedronBall::OctahedronBall(int n) : m_rekursjoner(n), m_indeks(0), GameObject()
{
   getMeshComp()->mVertices.reserve(3 * 8 * pow(4, m_rekursjoner));
   oktaederUnitBall();
}

OctahedronBall::~OctahedronBall()
{

}

void OctahedronBall::lagTriangel(const gsl::Vector3D& v1, const gsl::Vector3D& v2, const gsl::Vector3D& v3)
{
   Vertex v{};

   v.set_xyz(v1.x, v1.y, v1.z);		// koordinater v.x = v1.x, v.y=v1.y, v.z=v1.z
   v.set_normal(v1.x, v1.y, v1.z);	// rgb
   v.set_st(0.0f, 0.0f);			// kan utelates
   getMeshComp()->mVertices.push_back(v);
   v.set_xyz(v2.x, v2.y, v2.z);
   v.set_normal(v2.x, v2.y, v2.z);
   v.set_st(1.0f, 0.0f);
   getMeshComp()->mVertices.push_back(v);
   v.set_xyz(v3.x, v3.y, v3.z);
   v.set_normal(v3.x, v3.y, v3.z);
   v.set_st(0.5f, 1.0f);
   getMeshComp()->mVertices.push_back(v);
}

void OctahedronBall::subDivide(const gsl::Vector3D& a, const gsl::Vector3D& b, const gsl::Vector3D& c, int n)
{
   if (n>0) {
       gsl::Vector3D v1 = a+b; v1.normalize();
       gsl::Vector3D v2 = a+c; v2.normalize();
       gsl::Vector3D v3 = c+b; v3.normalize();
       subDivide(a, v1, v2, n-1);
       subDivide(c, v2, v3, n-1);
       subDivide(b, v3, v1, n-1);
       subDivide(v3, v2, v1, n-1);
   } else {
       lagTriangel(a, b, c);
   }
}

void OctahedronBall::oktaederUnitBall()
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

void OctahedronBall::init()
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &getMeshComp()->mVAO );
    glBindVertexArray( getMeshComp()->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &getMeshComp()->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, getMeshComp()->mVBO );

    glBufferData( GL_ARRAY_BUFFER, getMeshComp()->mVertices.size()*sizeof(Vertex), getMeshComp()->mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)( 6 * sizeof( GLfloat ) ));
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &getMeshComp()->mEAB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getMeshComp()->mEAB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getMeshComp()->mIndices.size() * sizeof(GLuint), getMeshComp()->mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void OctahedronBall::draw()
{
   glBindVertexArray( getMeshComp()->mVAO );
   glUniformMatrix4fv( getMeshComp()->mMatrixUniform, 1, GL_TRUE, getTransformComp()->mMatrix.constData());
   glDrawArrays(GL_TRIANGLES, 0, getMeshComp()->mVertices.size());//mVertices.size());
}

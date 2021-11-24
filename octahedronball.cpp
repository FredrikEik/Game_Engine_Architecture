#include "octahedronball.h"
#include "vertex.h"

OctahedronBall::OctahedronBall(int n) : m_rekursjoner(n), m_indeks(0), GameObject()
{
   getMeshComponent()->mVertices.reserve(3 * 8 * pow(4, m_rekursjoner));
   oktaederUnitBall();
}

OctahedronBall::~OctahedronBall()
{

}


void OctahedronBall::lagTriangel(const Vec3 &v1, const Vec3 &v2, const Vec3 &v3)
{
    Vertex v{};

    v.set_xyz(v1.x, v1.y, v1.z);		// koordinater v.x = v1.x, v.y=v1.y, v.z=v1.z
    v.set_normal(v1.x, v1.y, v1.z);	// rgb
    v.set_st(0.0f, 0.0f);			// kan utelates
    getMeshComponent()->mVertices.push_back(v);
    v.set_xyz(v2.x, v2.y, v2.z);
    v.set_normal(v2.x, v2.y, v2.z);
    v.set_st(1.0f, 0.0f);
    getMeshComponent()->mVertices.push_back(v);
    v.set_xyz(v3.x, v3.y, v3.z);
    v.set_normal(v3.x, v3.y, v3.z);
    v.set_st(0.5f, 1.0f);
    getMeshComponent()->mVertices.push_back(v);
}

void OctahedronBall::subDivide(const Vec3 &a, const Vec3 &b, const Vec3 &c, int n)
{
    if (n>0) {
        Vec3 v1 = a+b; v1.normalize();
        Vec3 v2 = a+c; v2.normalize();
        Vec3 v3 = c+b; v3.normalize();
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
    Vec3 v0{0, 0, 1};
    Vec3 v1{1, 0, 0};
    Vec3 v2{0, 1, 0};
    Vec3 v3{-1, 0, 0};
    Vec3 v4{0, -1, 0};
    Vec3 v5{0, 0, -1};

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
    initializeOpenGLFunctions();

       // Set what shader you want to use to render this object
       //mMaterial->setActiveShader(ShaderType::TEXTURE_SHADER);
       //mMaterial->setActiveTextureSlot(2);
       //mMaterial->setupModelMatrixUniform(mMatrixUniform, matrixUniform);

       glGenVertexArrays( 1, &getMeshComponent()->mVAO );
       glBindVertexArray( getMeshComponent()->mVAO );


       glGenBuffers( 1, &getMeshComponent()->mVBO );
       glBindBuffer( GL_ARRAY_BUFFER, getMeshComponent()->mVBO );

       glBufferData( GL_ARRAY_BUFFER,                     //what buffer type
                     getMeshComponent()->mVertices.size() * sizeof( Vertex ), //how big buffer do we need
                     getMeshComponent()->mVertices.data(),                    //the actual vertices
                     GL_STATIC_DRAW                       //should the buffer be updated on the GPU
                     );

       glBindBuffer(GL_ARRAY_BUFFER, getMeshComponent()->mVBO);

       // 1rst attribute buffer : coordinates

       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(0));
       glEnableVertexAttribArray(0);

       // 2nd attribute buffer : colors
       glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)) );
       glEnableVertexAttribArray(1);

       // 3rd attribute buffer : uvs
       glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
       glEnableVertexAttribArray(2);

       glBindVertexArray(0);

}

void OctahedronBall::draw()
{
    glBindVertexArray( getMeshComponent()->mVAO );
    glDrawArrays(getMeshComponent()->mDrawType, 0, getMeshComponent()->mVertices.size());
    glBindVertexArray(0);
}

void OctahedronBall::move(float x, float y, float z)
{
    getTransformComponent()->mMatrix.translate(x,y,z);
    getSphereCollisionComponent()->center += gsl::Vector3D(x,y,z);
    //getBoxCollisionComponent()->min += gsl::Vector3D(0.001f,0.001f, -0.001f);
    //getBoxCollisionComponent()->max += gsl::Vector3D(0.001f,0.001f, -0.001f);
}

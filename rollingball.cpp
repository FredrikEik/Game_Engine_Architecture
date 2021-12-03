#include "rollingball.h"
#include "lasplane.h"
#include "mathfunctions.h"
#include "constants.h"
#include "Shaders/shader.h"
#include "ECS/ResourceManager/resourcemanager.h"

RollingBall::RollingBall(int loadedMeshIndex)
{
    m_scale.scale(0.25,0.25,0.25);
    meshIndex = loadedMeshIndex;
}

RollingBall::~RollingBall()
{

}

void RollingBall::move(float dt)
{
    //std::vector<gsml::Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

    //QVector3D pos = m_position.getPosition(); //use for function call for bary.
    QVector3D pos = m_position.row(3).toVector3D();
    float height = getHeight(pos) + radius;

    if(current_index != old_index)
        doCollition();

    velocity = velocity + acceleration * dt;
    QVector3D newPos = pos + velocity * dt;
    newPos.setZ(height);

    //m_position.setPosition(newPos);
    m_position.setRow(3, QVector4D(newPos, 1.f));
    m_modelMatrix = m_position * m_scale;

    old_index = current_index;
    old_normal = current_normal;
}

bool RollingBall::findTriangle(unsigned int index, QVector3D &position, QVector3D &outBaryCords, QVector3D &outP, QVector3D &outQ, QVector3D &outR) const
{
    QVector3D p, q, r;
    std::vector<Vertex>& vertices = dynamic_cast<LasPlane*>(las_plane)->get_vertices();


//    qDebug()<<"SIZE: "<<m_indices.size();
//    p = vertices[m_indices[index]].getPosition();
//    q = vertices[m_indices[index+1]].getPosition();
//    r = vertices[m_indices[index+2]].getPosition();

    if((index+2)>=vertices.size())
        return false;

    p = vertices[index].getPosition();
    q = vertices[index+1].getPosition();
    r = vertices[index+2].getPosition();

    //outBaryCords = position.barycentricCoordinates(p, q, r);
    outBaryCords = gsm::MathFunctions::barycentricCoordinates(position.toVector2D(), p.toVector2D(), q.toVector2D(), r.toVector2D());

    if (outBaryCords.x() >= 0 && outBaryCords.y() >= 0 && outBaryCords.z() >= 0)
    {
        outP = vertices[index].getPosition();
        outQ = vertices[index+1].getPosition();
        outR = vertices[index+2].getPosition();
//        outP = vertices[m_indices[index]].getPosition();
//        outQ = vertices[m_indices[index+1]].getPosition();
//        outR = vertices[m_indices[index+2]].getPosition();
        return true;
    }

    return false;
}

float RollingBall::getHeight(QVector3D& positon)
{
    QVector3D BaryCords, p, q, r;
    //std::vector<Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();
    std::vector<Vertex>& vertices = dynamic_cast<LasPlane*>(las_plane)->get_vertices();

    for (unsigned int i{}; i < vertices.size(); i += 3)
    {
        if (findTriangle(i, positon, BaryCords, p, q, r))
        {
            current_index = i;
            RollingBall::calculateMovement(p, q, r);
            break;
        }
    }

    return BaryCords.x() * p.z() + BaryCords.y() * q.z() + BaryCords.z() * r.z();

}

void RollingBall::calculateMovement(const QVector3D& p, const QVector3D& q, const QVector3D& r)
{
    //Normal
    QVector3D pq = q - p;
    QVector3D pr = r - p;
    //QVector3D normal = pq ^ pr;
    QVector3D normal = QVector3D::crossProduct(pq, pr);
    normal.normalize();
    current_normal = normal;

    //qDebug() << normal.x << " " << normal.y << " " << normal.z;

    acceleration = QVector3D((normal.x() * normal.z()) * -gravity, (normal.y() * normal.z()) * -gravity, (normal.z() * normal.z() - 1) * -gravity);
}

void RollingBall::doCollition()
{
    QVector3D collisonNormal = (old_normal + current_normal)/(old_normal.length() + current_normal.length());
    velocity = velocity - collisonNormal * 2.f * (velocity * collisonNormal);
}

void RollingBall::init(GLint matrixUniform, const std::vector<Vertex> &vertices, const std::vector<Vertex> &indices)
{
    m_matrixUniform = matrixUniform;

    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // 1st attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors (normals)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : UV.
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    //Indices Buffer Object - IBO
    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}


void RollingBall::draw(Meshdata& meshData)
{
    initializeOpenGLFunctions();
    glBindVertexArray(m_VAO);

    glUniformMatrix4fv(m_shader->getModelMatrixUniform(), 1, GL_FALSE, m_modelMatrix.constData());

    glUniform3fv(m_shader->getObjectColorUniform(), 1, &m_objectColor[0]);
    glUniform1f(m_shader->getSpecularStrengthUniform(), m_specularStrenght);
    glUniform1i(m_shader->getSpecularExponentUniform(), m_specularExponent);
    glUniform1f(m_shader->getObjectAlphaUniform(), m_objectAlpha);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
}

void RollingBall::init(GLint matrixUniform)
{

}

void RollingBall::draw()
{

}


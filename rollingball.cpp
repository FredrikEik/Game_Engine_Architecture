#include "rollingball.h"
#include "lasplane.h"
#include "mathfunctions.h"

RollingBall::RollingBall(int n) : OctahedronBall (n)
{
    m_position.translate(-0.5,-0.5,1.0);
    m_scale.scale(0.25,0.25,0.25);
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

void RollingBall::init(GLint matrixUniform)
{
   m_matrixUniform = matrixUniform;
   initializeOpenGLFunctions();

   //Vertex Array Object - VAO
   glGenVertexArrays( 1, &m_VAO );
   glBindVertexArray( m_VAO );

   //Vertex Buffer Object to hold vertices - VBO
   glGenBuffers( 1, &m_VBO );
   glBindBuffer( GL_ARRAY_BUFFER, m_VBO );

   glBufferData( GL_ARRAY_BUFFER, m_vertices.size()*sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW );

   glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
   glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), (GLvoid*)0);
   glEnableVertexAttribArray(0);

   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
   glEnableVertexAttribArray(1);

   glBindVertexArray(0);
}

void RollingBall::draw()
{
   glBindVertexArray( m_VAO );
   glUniformMatrix4fv( m_matrixUniform, 1, GL_TRUE, m_modelMatrix.constData());
   glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
}



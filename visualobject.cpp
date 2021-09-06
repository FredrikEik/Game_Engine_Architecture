#include "visualobject.h"
#include "shader.h"
#include "texture.h"
#include <QDebug>

VisualObject::VisualObject()
{
    m_scale.setToIdentity();
    m_scale.scale(QVector3D(1,1,1));
    m_modelMatrix.setToIdentity();
    m_rotation.setToIdentity();
    m_position.setToIdentity();
}

VisualObject::~VisualObject()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void VisualObject::writeFile(const std::string &filename)
{
    std::ofstream outStream;
    outStream.open(filename.c_str());

    if (outStream.is_open())
    {
        int size = m_vertices.size();
        Vertex tempVertex;

        outStream << size << std::endl;
        for (int i{0}; i<size; i++)
        {
            tempVertex = m_vertices.at(i);
            outStream << tempVertex << std::endl;
        }
        outStream.close();
    } else
        qDebug() << "Error, could not write to file!";
}


void VisualObject::readFile(const std::string &filename)
{
    std::ifstream inStream;
    inStream.open(filename.c_str());

    if (inStream.is_open())
    {
        int size{};
        Vertex tempVertex;

        inStream>>size;
        m_vertices.reserve(size);

        while (inStream)
        {
            inStream>>tempVertex;
            m_vertices.push_back(tempVertex);
        }
        inStream.close();
    } else
        qDebug() << "Can't open read file: " << &filename;

}

void VisualObject::updateModelMatrix()
{
    m_modelMatrix = m_position * m_rotation * m_scale;
}

void VisualObject::setObjectName(const std::string &name)
{
    m_objectname = name;
}

std::string VisualObject::getObjectName() const
{
    return m_objectname;
}

void VisualObject::setPosition(const QVector3D &position)
{
    m_position.setToIdentity();
    m_position.translate(position);

    updateModelMatrix();
}

void VisualObject::setPosition(const QMatrix4x4 &position)
{
    m_position = position;

    updateModelMatrix();
}

QVector3D VisualObject::getPosition() const
{
    return m_position.column(3).toVector3D();
}

void VisualObject::setScale(const QVector3D &scale)
{
    m_scale.setToIdentity();
    m_scale.scale(scale);

    updateModelMatrix();
}

void VisualObject::setScale(const QMatrix4x4 &scale)
{
    m_scale = scale;
}

QVector3D VisualObject::getScale() const
{
    return QVector3D(m_scale.column(0).x(), m_scale.column(1).y(), m_scale.column(2).z());
}

void VisualObject::setRotation(const QVector3D &rotation)
{
    m_rotation.setToIdentity();
    m_rotation.rotate(45.f, rotation);

    updateModelMatrix();
}

QMatrix4x4 VisualObject::getRotation() const
{
    return m_rotation;
}

QMatrix4x4 VisualObject::getModelMatrix() const
{
    return m_modelMatrix;
}

std::string VisualObject::getShaderName() const
{
    return m_shaderName;
}

Shader *VisualObject::getShader() const
{
    return m_shader;
}

Texture *VisualObject::getTexture() const
{
    return m_texture;
}

void VisualObject::setTexture(Texture *texture)
{
    m_texture = texture;
}

void VisualObject::setShader(Shader *shader)
{
    m_shader = shader;
}

void VisualObject::setShaderName(const std::string &shaderName)
{
    m_shaderName = shaderName;
}

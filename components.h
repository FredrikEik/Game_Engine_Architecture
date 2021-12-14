#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <QOpenGLFunctions_4_1_Core>
#include <qmatrix4x4.h>
#include "vertex.h"

using ComponentType = GLubyte;
const ComponentType MAX_COMPONENTS = 16;

using Entity = GLuint;
const Entity MAX_ENTITIES = 1000;

struct TransformComponent
{
    QMatrix4x4 m_modelMatrix{};
    QMatrix4x4 m_position{};
    QMatrix4x4 m_rotation{};
    QMatrix4x4 m_scale{};
    QVector3D m_velocity{};

    GLint m_matrixUniform{};
};

struct MeshComponent
{
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    GLuint m_VAO[3];  //VertexArrayObject
    GLuint m_VBO;  //VertexBufferObject
    GLuint m_IBO;  //IndexBufferObject
    GLenum m_drawType{GL_TRIANGLES};
};

struct MaterialComponent
{
    std::string m_shaderName;
    QVector3D m_objectColor;
    int m_specularExponent;
    float m_specularStrenght;
    float m_objectAlpha;

    GLuint m_shaderProgram;
    GLint m_textureUnit;
};

#endif // COMPONENTS_H

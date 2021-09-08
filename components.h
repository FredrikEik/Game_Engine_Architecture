#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <QOpenGLFunctions_4_1_Core>
#include <qmatrix4x4.h>
#include "vertex.h"

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
    GLuint m_VAO;  //VertexArrayObject
    GLuint m_VBO;  //VertexBufferObject
    GLuint m_IBO;  //IndexBufferObject
};

struct MaterialComponent
{
    QVector3D m_objectColor;
    int m_specularExponent;
    float m_specularStrenght;
    float m_objectAlpha;

    GLuint m_shaderProgram;
    GLint m_textureUnit;
};

//struct ShaderComponent
//{
//    std::string m_shaderName; //Might not need this?
//    class Shader* m_shader;
//};

//struct TextureComponent
//{
//    std::string m_textureName; //Might not need this?
//    class Texture* m_texture;
//};

#endif // COMPONENTS_H

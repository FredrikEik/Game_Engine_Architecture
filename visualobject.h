#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include "vertex.h"

class RenderWindow;
class Shader;
class Texture;

class VisualObject : public QOpenGLFunctions_4_1_Core
{
public:
    VisualObject();
    virtual ~VisualObject();

    virtual void init(GLint matrixUniform) = 0;
    virtual void draw() = 0;
    void readFile(const std::string &filename);
    virtual void writeFile(const std::string &filename);
    virtual void updateModelMatrix();

    void setObjectName(const std::string &name);
    void setPosition(const QVector3D &position);
    void setPosition(const QMatrix4x4 &position);
    void setScale(const QVector3D &scale);
    void setScale(const QMatrix4x4 &scale);
    void setRotation(const QVector3D &rotation);
    void setShaderName(const std::string &shaderName);
    void setShader(Shader *shader);
    void setTexture(Texture *texture);

    std::string getObjectName() const;
    QVector3D getPosition() const;
    QVector3D getFirstVertexPosition();
    QVector3D getScale() const;
    QMatrix4x4 getRotation() const;
    QMatrix4x4 getModelMatrix() const;
    std::string getShaderName() const;
    Shader *getShader() const;
    Texture *getTexture() const;


protected:
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    GLuint m_VAO{};  //VertexArrayObject
    GLuint m_VBO{};  //VertexBufferObject
    GLuint m_IBO{};  //IndexBufferObject

    GLint m_matrixUniform{};

    QMatrix4x4 m_modelMatrix{};
    QMatrix4x4 m_position{};
    QMatrix4x4 m_rotation{};
    QMatrix4x4 m_scale{};
    QVector3D m_velocity{};

    QVector3D m_objectColor{0.7f, 0.7f, 0.7f}; //Grey
    int m_specularExponent{32};
    float m_specularStrenght{0.5f};
    float m_objectAlpha{1.f};

    std::string m_objectname;
    std::string m_shaderName; //Used to retrieve the correct shader program from the shader map.
    Shader* m_shader;
    Texture* m_texture;
};
#endif // VISUALOBJECT_H


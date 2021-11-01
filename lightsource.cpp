#include "lightsource.h"
#include "Shaders/shader.h"

LightSource::LightSource(Shader* shader)
    :VisualObject()
{
    m_objectColor = (QVector3D(0.7f, 0.7f, 0.3f));
    m_shader = shader;
    m_lightColor = {1.f, 1.f, 1.f};

    //Cube
    m_vertices.push_back(Vertex{-1,-1,-1,0,0,0});   //0
    m_vertices.push_back(Vertex{1,-1,-1,0,0,0});    //1
    m_vertices.push_back(Vertex{1,1,-1,1,0,1});     //2
    m_vertices.push_back(Vertex{-1,1,-1,1,0,0});    //3
    m_vertices.push_back(Vertex{-1,-1,1,0,0,0});    //4
    m_vertices.push_back(Vertex{1,-1,1,0,0,0});     //5
    m_vertices.push_back(Vertex{1,1,1,1,0,0});      //6
    m_vertices.push_back(Vertex{-1,1,1,1,0,0});     //7

    //Towards camera
    m_indices.push_back(0);
    m_indices.push_back(1);
    m_indices.push_back(3);
    m_indices.push_back(3);
    m_indices.push_back(1);
    m_indices.push_back(2);

    //Perpendicular to the one above
    m_indices.push_back(5);
    m_indices.push_back(4);
    m_indices.push_back(6);
    m_indices.push_back(6);
    m_indices.push_back(4);
    m_indices.push_back(7);

    //Back
    m_indices.push_back(4);
    m_indices.push_back(0);
    m_indices.push_back(7);
    m_indices.push_back(7);
    m_indices.push_back(0);
    m_indices.push_back(3);

    //Top
    m_indices.push_back(3);
    m_indices.push_back(2);
    m_indices.push_back(7);
    m_indices.push_back(7);
    m_indices.push_back(2);
    m_indices.push_back(6);

    //Bottom
    m_indices.push_back(4);
    m_indices.push_back(5);
    m_indices.push_back(0);
    m_indices.push_back(0);
    m_indices.push_back(5);
    m_indices.push_back(1);

    m_modelMatrix.setToIdentity();
}

void LightSource::init(GLint matrixUniform)
{
    m_matrixUniform = matrixUniform;
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size()*sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), reinterpret_cast<const void*>(0));
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),  reinterpret_cast<const void*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : UV.
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  reinterpret_cast<const void*>(6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

}

void LightSource::draw()
{
    setPosition(QVector3D(50.f, 50.f, 50.f));

    initializeOpenGLFunctions();
    glBindVertexArray(m_VAO);

    m_lightPosition = getPosition();

    glUniform3fv(m_shader->getLightColorUniform(), 1, &m_lightColor[0]);
    glUniform3fv(m_shader->getLightPositionUniform(), 1, &m_lightPosition[0]);
    glUniform1f(m_shader->getAmbientStrengthUniform(), m_ambientStrengt);
    glUniform1f(m_shader->getLightStrengthUniform(), m_lightStrengt);
    glUniform3fv(m_shader->getObjectColorUniform(), 1, &m_objectColor[0]);

    glUniformMatrix4fv(m_shader->getModelMatrixUniform(), 1, GL_FALSE, m_modelMatrix.constData());

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
}


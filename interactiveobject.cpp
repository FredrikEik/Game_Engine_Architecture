#include "interactiveobject.h"

InteractiveObject::InteractiveObject()
    :VisualObject()
{

}

InteractiveObject::InteractiveObject(const std::string &filename)
    :VisualObject()
{
    readFile(filename);
    m_modelMatrix.setToIdentity();
}

void InteractiveObject::init(GLint matrixUniform)
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

    glBindVertexArray(0);
}

void InteractiveObject::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray(m_VAO);
    glUniformMatrix4fv(m_matrixUniform, 1, GL_FALSE, m_modelMatrix.constData());

    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
}

void InteractiveObject::move(QVector3D inVector)
{
    m_position.translate(inVector);

    updateModelMatrix();
}

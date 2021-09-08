#include "xyz.h"
#include "vertex.h"
#include "shader.h"

XYZ::XYZ() {
    m_vertices.push_back(Vertex{0.f, 0.f, 0.f, 1.f, 0.f, 0.f});
    m_vertices.push_back(Vertex{100.f, 0.f, 0.f, 1.f, 0.f, 0.f});
    m_vertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 1.f, 0.f});
    m_vertices.push_back(Vertex{0.f, 100.f, 0.f, 0.f, 1.f, 0.f});
    m_vertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 0.f, 1.f});
    m_vertices.push_back(Vertex{0.f, 0.f, 100.f, 0.f, 0.f, 1.f});
    m_modelMatrix.setToIdentity();
}

XYZ::~XYZ()
{
}

void XYZ::init(GLint matrixUniform)
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &m_VAO );
    glBindVertexArray( m_VAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &m_VBO );
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );

    glBufferData( GL_ARRAY_BUFFER, m_vertices.size()*sizeof( Vertex ), m_vertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void XYZ::draw()
{
    glBindVertexArray( m_VAO );
    glDrawArrays(GL_LINES, 0, m_vertices.size());
}

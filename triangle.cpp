#include "triangle.h"
#include "vertex.h"

Triangle::Triangle()
{
                                // Positions            // Colors       //UV
    m_vertices.push_back(Vertex{-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  0.f, 0.f}); // Bottom Left
    m_vertices.push_back(Vertex{0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.f}); // Bottom Right
    m_vertices.push_back(Vertex{0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.f}); // Top
    m_modelMatrix.setToIdentity();
}

Triangle::~Triangle()
{
}

void Triangle::init()
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    glGenVertexArrays( 1, &m_VAO );
    glBindVertexArray( m_VAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &m_VBO );
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, m_vertices.size()*sizeof( Vertex ), m_vertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0  );          // array buffer offset
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Triangle::draw()
{
    glBindVertexArray( m_VAO );
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

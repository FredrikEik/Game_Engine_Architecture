#include "skybox.h"
#include "cubemap.h"
#include "Shaders/shader.h"

SkyBox::SkyBox()
    :VisualObject()
{
    float scale{0.5f};
    m_vertices.push_back(Vertex(-scale, -scale, -scale, 0, 0, 1));
    m_vertices.push_back(Vertex(scale, -scale, -scale, 0, 0, 1));
    m_vertices.push_back(Vertex(scale, scale, -scale, 0, 0, 1));
    m_vertices.push_back(Vertex(-scale, scale, -scale, 0, 0, 1));
    m_vertices.push_back(Vertex(-scale, -scale, scale, 0, 0, 1));
    m_vertices.push_back(Vertex(scale, -scale, scale, 0, 0, 1));
    m_vertices.push_back(Vertex(scale, scale, scale, 0, 0, 1));
    m_vertices.push_back(Vertex(-scale, scale, scale, 0, 0, 1));

    m_indices.push_back(0);
    m_indices.push_back(1);
    m_indices.push_back(3);
    m_indices.push_back(3);
    m_indices.push_back(1);
    m_indices.push_back(2);

    m_indices.push_back(1);
    m_indices.push_back(5);
    m_indices.push_back(2);
    m_indices.push_back(2);
    m_indices.push_back(5);
    m_indices.push_back(6);

    m_indices.push_back(5);
    m_indices.push_back(4);
    m_indices.push_back(6);
    m_indices.push_back(6);
    m_indices.push_back(4);
    m_indices.push_back(7);

    m_indices.push_back(4);
    m_indices.push_back(0);
    m_indices.push_back(7);
    m_indices.push_back(7);
    m_indices.push_back(0);
    m_indices.push_back(3);

    m_indices.push_back(3);
    m_indices.push_back(2);
    m_indices.push_back(7);
    m_indices.push_back(7);
    m_indices.push_back(2);
    m_indices.push_back(6);

    m_indices.push_back(4);
    m_indices.push_back(5);
    m_indices.push_back(0);
    m_indices.push_back(0);
    m_indices.push_back(5);
    m_indices.push_back(1);

    std::array<std::string, 6> skyboxPaths{};
    for(unsigned int i{}; i<skyboxPaths.size(); ++i)
    {
        std::string path{"../3DProg21/Assets/Skybox/"};
        std::string suffix{".bmp"};
        skyboxPaths[i] = path.append(std::to_string(i+1)).append(suffix);
    }

    m_cubemap = new Cubemap(skyboxPaths);
}

SkyBox::~SkyBox()
{
    delete m_cubemap;
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void SkyBox::init(GLint matrixUniform)
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
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), reinterpret_cast<const void*>(0));
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

}


void SkyBox::draw()
{
    initializeOpenGLFunctions();

    glDepthMask(GL_FALSE);

    glBindVertexArray(m_VAO);

    if(m_cubemap)
    {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemap->id());
        glUniform1i(m_shader->getTextureUniform(), 2);
    }

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));

    glDepthMask(GL_TRUE);

}

#include "cubemap.h"

Cubemap::Cubemap(const std::array<std::string, 6> &fileNames)
    :m_fileNames(fileNames)
{
    initializeOpenGLFunctions();
    readCubemap();
    setTexture();
}

void Cubemap::readCubemap()
{
    for(unsigned long long i{}; i<m_fileNames.size(); ++i)
    {
        readBitmap(m_fileNames[i]);
        m_bitmaps[i] = m_bitmap;
    }
}

void Cubemap::setTexture()
{
    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &m_GLTextureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_GLTextureId);

    for(unsigned int i{}; i<m_bitmaps.size();++i)
    {
        glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0,                  //mipmap level
                    GL_RGB,             //internal format - what format should OpenGL use
                    m_columns,
                    m_rows,
                    0,                  //always 0
                    GL_BGR,             //format of data from texture file -  bmp uses BGR, not RGB
                    GL_UNSIGNED_BYTE,   //size of each color channel
                    m_bitmaps[i]);           //pointer to texture in memory
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

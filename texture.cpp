#include <QDebug>
#include <QImage>
#include <QBuffer>
#include <QByteArray>

#include <iostream>
#include <fstream>
#include <string>

#include "texture.h"

Texture::Texture()
{
    initializeOpenGLFunctions();
    //small dummy texture
    for (int i=0; i<16; i++)
        m_pixels[i] = 0;
    m_pixels[0] = 255;
    m_pixels[4] = 255;
    m_pixels[8] = 255;
    m_pixels[9] = 255;
    m_pixels[10] = 255;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &m_GLTextureId);
    glBindTexture(GL_TEXTURE_2D, m_GLTextureId);
    qDebug() << "Texture default: id = " << m_GLTextureId;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 reinterpret_cast<const GLvoid*>(m_pixels));
}

/**
 \brief Texture::Texture() Read a bitmap file and create a texture with standard parameters
 \param filename The name of the bmp file containing a texture
 First one 2D texture is generated from
 - glGenTextures()
 Then the OpenGL functions
 - glBindTexture()
 - glTexParameteri()
 - glTexImage2D()
 are used. The texture can be retrieved later by using the function id()
 */
Texture::Texture(const std::string& filename): QOpenGLFunctions_4_1_Core()
{
    textureFilename = filename;
    initializeOpenGLFunctions();
    readBitmap(filename);       //reads the BMP into memory
    setTexture();               //set texture up for OpenGL
}

Texture::~Texture()
{
    delete[] m_bitmap;
}

/**
    \brief Texture::id() Return the id of a previously generated texture object
    \return The id of a previously generated texture object
 */
GLuint Texture::id() const
{
    return m_GLTextureId;
}

int Texture::rows() const
{
    return m_rows;
}

int Texture::columns() const
{
    return m_columns;
}

unsigned char *Texture::bitmap() const
{
    return m_bitmap;
}

int Texture::bytesPrPixel() const
{
    return m_bytesPrPixel;
}

//Read BMP into memory
void Texture::readBitmap(const std::string &filename)
{
    OBITMAPFILEHEADER bmFileHeader;
    OBITMAPINFOHEADER bmInfoHeader;

    std::string fileWithPath = filename;

    std::ifstream file;
    file.open (fileWithPath.c_str(), std::ifstream::in | std::ifstream::binary);
    if (file.is_open())
    {
        file.read((char *) &bmFileHeader, 14);
        file.read((char *) &bmInfoHeader, sizeof(OBITMAPINFOHEADER));
        m_columns = bmInfoHeader.biWidth;
        m_rows = bmInfoHeader.biHeight;
        m_bytesPrPixel = bmInfoHeader.biBitCount / 8;
        if(m_bytesPrPixel == 4)
            m_alphaUsed = true;
        qDebug() << "hello, is me! "<<QString::fromStdString( filename)<<" col: "<<m_columns<<" rows: "<<m_rows<<" bytes: "<<m_bytesPrPixel;

        m_bitmap = new unsigned char[m_columns * m_rows * m_bytesPrPixel];
        file.read((char *) m_bitmap, m_columns * m_rows * m_bytesPrPixel);
        file.close();
    }
    else
    {
        qDebug() << "ERROR: Can not read " << QString(fileWithPath.c_str());
    }
}

//Set up texture for use in OpenGL
void Texture::setTexture()
{
    glGenTextures(1, &m_GLTextureId);
    glBindTexture(GL_TEXTURE_2D, m_GLTextureId);
    qDebug() << "Texture" << textureFilename.c_str() << "successfully read | id = " << m_GLTextureId << "| bytes pr pixel:" << m_bytesPrPixel << "| using alpha:" << m_alphaUsed;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    if(m_alphaUsed == false)                     //no alpha in this bmp
        glTexImage2D(
                    GL_TEXTURE_2D,
                    0,                  //mipmap level
                    GL_RGB,             //internal format - what format should OpenGL use
                    m_columns,
                    m_rows,
                    0,                  //always 0
                    GL_BGR,             //format of data from texture file -  bmp uses BGR, not RGB
                    GL_UNSIGNED_BYTE,   //size of each color channel
                    m_bitmap);           //pointer to texture in memory

    else                                //alpha is present, so we set up an alpha channel
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_columns, m_rows, 0,  GL_BGRA, GL_UNSIGNED_BYTE, m_bitmap);

    glGenerateMipmap(GL_TEXTURE_2D);
}

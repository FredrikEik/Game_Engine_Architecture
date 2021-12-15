#include <QDebug>
#include <QImage>
#include <QBuffer>
#include <QByteArray>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "texture.h"
#include "constants.h"

Textures::Textures()
{
//    initializeOpenGLFunctions();
//    //small dummy texture
//    mBitmap = new unsigned char[16];
//    for (int i=0; i<16; i++)
//        mBitmap[i] = 0;

//    mBitmap[0] = 255;
//    mBitmap[4] = 255;
//    mBitmap[8] = 255;
//    mBitmap[9] = 255;
//    mBitmap[10] = 255;

//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

//    glGenTextures(1, &mGLTextureID);
//    glBindTexture(GL_TEXTURE_2D, mGLTextureID);
//    qDebug() << "Texture default: id = " << mGLTextureID;
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, mBitmap);

//    mTextureFilename = "Basic Texture";
}

int Textures::makeTexture(const std::string &filename)
{
    initializeOpenGLFunctions();

    int textureIndex{-1};

    //check if asset is made:
    auto result = mTextureMap.find(filename);
    //if already made
    if (result != mTextureMap.end()) {        //found!!!
        qDebug() << filename.c_str() << "made already";
        textureIndex = result->second;
    }
    else {
        mTextures.emplace_back(Texture());
        if (filename.find(".bmp") != std::string::npos)
        {
                textureIndex = readBitmap(filename);
        }
        if(filename == "none")
            textureIndex = makeDefault();

        //update mTextureMap with new asset
        mTextureMap.emplace(filename, textureIndex);
    }

    //Set the textures loaded to a texture unit
    glActiveTexture(GL_TEXTURE0 + textureIndex);
    glBindTexture(GL_TEXTURE_2D, mTextures.at(textureIndex).mGLTextureID);

    return textureIndex;
}

float Textures::getHeightFromIndex(int i)
{
    if (i > mColumns * mRows || i < 0)
        return 0;

    int r = mBitmap[i*3];
    int g = mBitmap[i*3+1];
    int b = mBitmap[i*3+2];

    float height = (r+g+b)/3.f;

    return height;
}

//Read BMP into memory
int Textures::readBitmap(const std::string &filename)
{
    OBITMAPFILEHEADER bmFileHeader;
    OBITMAPINFOHEADER bmInfoHeader;

    std::string fileWithPath = gsl::TextureFilePath + filename;

    std::ifstream file;
    file.open (fileWithPath.c_str(), std::ifstream::in | std::ifstream::binary);
    if (file.is_open())
    {
        file.read((char *) &bmFileHeader, 14);
        if (bmFileHeader.bfType != 19778)   // 19778 equals "BM"
        {
            qDebug() << "ERROR:" << QString(fileWithPath.c_str()) << "is not a bitmap file!";
            return -1;
        }

        file.read((char *) &bmInfoHeader, sizeof(OBITMAPINFOHEADER));
        mColumns = bmInfoHeader.biWidth;
        mRows = bmInfoHeader.biHeight;
        mBytesPrPixel = bmInfoHeader.biBitCount / 8;
        if(mBytesPrPixel == 4)
            mAlphaUsed = true;

        mBitmap = new unsigned char[mColumns * mRows * mBytesPrPixel];
        file.read((char *) mBitmap, mColumns * mRows * mBytesPrPixel);
        file.close();
        qDebug() << "Texture:" << QString(fileWithPath.c_str()) << "found!";
    }
    else
    {
        qDebug() << "ERROR: Can not read " << QString(fileWithPath.c_str());
    }
    return -1;
}

void Textures::readCubeMap()
{
    std::string justName;
    std::stringstream sStream;
    sStream << mTextureFilename;
    std::getline(sStream, justName, '.');   //deleting .bmp
    justName.pop_back();    //removing 1
    for(int i{0}; i< 6; i++)
    {
        //TODO: clean this up! Decide where CubeMaps should be located
        std::string temp = "../CubeMaps/" +justName + std::to_string(i+1) + ".bmp";   //adding Cubemap path and 1 - 6 to filename
        readBitmap(temp);
        mCubemap[i] = mBitmap;
    }
}

//Set up texture for use in OpenGL
void Textures::setTexture()
{
    glGenTextures(1, &mGLTextureID);
    glBindTexture(GL_TEXTURE_2D, mGLTextureID);
    qDebug() << "Texture" << mTextureFilename.c_str() << "successfully read | id = " << mGLTextureID << "| bytes pr pixel:" << mBytesPrPixel << "| using alpha:" << mAlphaUsed << "| w:" << mColumns << "|h:" << mRows;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    if(!mAlphaUsed)                     //no alpha in this bmp
        glTexImage2D(
                    GL_TEXTURE_2D,
                    0,                  //mipmap level
                    GL_RGB,             //internal format - what format should OpenGL use
                    mColumns,
                    mRows,
                    0,                  //always 0
                    GL_BGR,             //format of data from texture file -  bmp uses BGR, not RGB
                    GL_UNSIGNED_BYTE,   //size of each color channel
                    mBitmap);           //pointer to texture in memory

    else                                //alpha is present, so we set up an alpha channel
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mColumns, mRows, 0,  GL_BGRA, GL_UNSIGNED_BYTE, mBitmap);

    glGenerateMipmap(GL_TEXTURE_2D);
}

void Textures::setCubemapTexture()
{
    glGenTextures(1, &mGLTextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mGLTextureID);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    if(!mAlphaUsed)  //no alpha in this bmp
        for(int i{0}; i< 6; i++)
        {
            glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, mColumns, mRows, 0, GL_BGR, GL_UNSIGNED_BYTE,  mCubemap[i]);
        }

    else    //alpha is present, so we set up an alpha channel
        qDebug() << "Skybox with alpha probably make no sense!?";

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    qDebug() << "Cubemap Texture" << mTextureFilename.c_str() << "successfully read | id = "
             << mGLTextureID << "| bytes pr pixel:" << mBytesPrPixel << "| using alpha:" << mAlphaUsed
             << "| w:" << mColumns << "|h:" << mRows;
}

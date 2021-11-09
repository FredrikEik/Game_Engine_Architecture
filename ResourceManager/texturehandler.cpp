#include <QDebug>
#include <QImage>
#include <QBuffer>
#include <QByteArray>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "texturehandler.h"
#include "constants.h"

TextureHandler::TextureHandler()
{}

int TextureHandler::makeTexture(const std::string &filename, bool cubeMap)
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
            if(cubeMap)
            {

                textureIndex = readCubeMap(filename);
            }

            else
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

int TextureHandler::makeDefault()
{
    Texture &temp = mTextures.back();
    //small dummy texture
    temp.mBitmap = new unsigned char[16];
    for (int i=0; i<16; i++)
        temp.mBitmap[i] = 0;

    temp.mBitmap[0] = 255;
    temp.mBitmap[4] = 255;
    temp.mBitmap[8] = 255;
    temp.mBitmap[9] = 255;
    temp.mBitmap[10] = 255;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &temp.mGLTextureID);
    glBindTexture(GL_TEXTURE_2D, temp.mGLTextureID);
    qDebug() << "Texture default: id = " << temp.mGLTextureID;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, temp.mBitmap);

    return mTextures.size()-1;    //returns index to last object
}

//Read BMP into memory
int TextureHandler::readBitmap(const std::string &filename)
{
    Texture &temp = mTextures.back();
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
        temp.mColumns = bmInfoHeader.biWidth;
        temp.mRows = bmInfoHeader.biHeight;
        temp.mBytesPrPixel = bmInfoHeader.biBitCount / 8;
        if(temp.mBytesPrPixel == 4)
            temp.mAlphaUsed = true;

        temp.mBitmap = new unsigned char[temp.mColumns * temp.mRows * temp.mBytesPrPixel];
        file.read((char *) temp.mBitmap, temp.mColumns * temp.mRows * temp.mBytesPrPixel);
        file.close();
        qDebug() << "Texture:" << QString(fileWithPath.c_str()) <<"read from file";
    }
    else
    {
        qDebug() << "ERROR: Can not read " << QString(fileWithPath.c_str());
    }

    setTexture(temp);

    return mTextures.size()-1;    //returns index to last object
}

//Set up texture for use in OpenGL
void TextureHandler::setTexture(Texture &textureIn)
{
    glGenTextures(1, &textureIn.mGLTextureID);
    glBindTexture(GL_TEXTURE_2D, textureIn.mGLTextureID);
    qDebug() << "Texture" << "successfully read | id = " << textureIn.mGLTextureID <<
                "| bytes pr pixel:" << textureIn.mBytesPrPixel << "| using alpha:" << textureIn.mAlphaUsed <<
                "| w:" << textureIn.mColumns << "|h:" << textureIn.mRows;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    if(!textureIn.mAlphaUsed)                     //no alpha in this bmp
        glTexImage2D(
                    GL_TEXTURE_2D,
                    0,                  //mipmap level
                    GL_RGB,             //internal format - what format should OpenGL use
                    textureIn.mColumns,
                    textureIn.mRows,
                    0,                  //always 0
                    GL_BGR,             //format of data from texture file -  bmp uses BGR, not RGB
                    GL_UNSIGNED_BYTE,   //size of each color channel
                    textureIn.mBitmap);           //pointer to texture in memory

    else                                //alpha is present, so we set up an alpha channel
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureIn.mColumns, textureIn.mRows, 0,
                     GL_BGRA, GL_UNSIGNED_BYTE, textureIn.mBitmap);

    glGenerateMipmap(GL_TEXTURE_2D);
}

int TextureHandler::readCubeMap(const std::string &filename)
{
    //TODO: This is not finished yet - will probably mess up mTextures vector
    Texture &tempTexture = mTextures.back();

    std::string justName;
    std::stringstream sStream;
    sStream << filename;
    std::getline(sStream, justName, '.');   //deleting .bmp
    justName.pop_back();    //removing 1
    for(int i{0}; i< 6; i++)
    {
        //TODO: clean this up! Decide where CubeMaps should be located
        std::string temp = justName + std::to_string(i+1) + ".bmp";
        readBitmap(temp);
        tempTexture.mCubemap[i] = tempTexture.mBitmap;
    }

     qDebug() << "Cubemap:" << QString(filename.c_str()) <<"loaded";

    setCubemapTexture(tempTexture);

    return mTextures.size()-1;    //returns index to last object
}
void TextureHandler::setCubemapTexture(Texture &textureIn)
{
    glGenTextures(1, &textureIn.mGLTextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureIn.mGLTextureID);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    if(!textureIn.mAlphaUsed)  //no alpha in this bmp
        for(int i{0}; i< 6; i++)
        {
            glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, textureIn.mColumns,
                          textureIn.mRows, 0, GL_BGR, GL_UNSIGNED_BYTE,  textureIn.mCubemap[i]);
        }

    else    //alpha is present, so we set up an alpha channel
        qDebug() << "Skybox with alpha probably make no sense!?";

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    qDebug() << "Cubemap Texture" << "successfully read | id = "
             << textureIn.mGLTextureID << "| bytes pr pixel:" << textureIn.mBytesPrPixel <<
                "| using alpha:" << textureIn.mAlphaUsed
             << "| w:" << textureIn.mColumns << "|h:" << textureIn.mRows;
}


float TextureHandler::getHeightFromIndex(int i)
{
    if (i > mColumns * mRows || i < 0)
        return 0;

    int r = mBitmap[i*3];
    int g = mBitmap[i*3+1];
    int b = mBitmap[i*3+2];

    float height = (r+g+b)/3.f;

    return height;
}

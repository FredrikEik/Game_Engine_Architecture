#include <QImage>
#include <QBuffer>
#include <QByteArray>

#include <sstream>

#include "ResourceManager/texturehandler.h"

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
//            if(cubeMap)
//                textureIndex = readCubeMap(filename);
//            else
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

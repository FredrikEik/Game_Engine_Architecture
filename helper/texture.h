#ifndef TEXTURE_H
#define TEXTURE_H

#include <QOpenGLFunctions_4_1_Core>

/**
 @class  Texture - Holder allle nødvendige funksjoner og variabler for texture.
   Klassen kan bl.a sette texture og skybox til objektene våres.
 */
class Texture : protected QOpenGLFunctions_4_1_Core
{
public:
    Texture();  //basic texture from code
    Texture(const std::string &filename, bool cubeMap = false);
    float getHeightFromIndex(int i);
    std::string mTextureFilename;

    int getColumns();
    int getRows();
    int mColumns{};
    int mRows{};
    unsigned char* getMap();
    unsigned char *mBitmap{nullptr};
    ///If this is a cubemap, this will be an array of 6 mBitmaps
    unsigned char *mCubemap[6]{nullptr};

    GLuint mGLTextureID{0};          //Texture ID that OpenGL makes when glGenTextures is called.

private:
    int mBytesPrPixel{};
    bool mAlphaUsed{false};
    void readBitmap(const std::string &filename);
    void readCubeMap();
    void setCubemapTexture();
    void setTexture();


    //this is put inside this class to avoid spamming the main namespace
    //with stuff that only is used inside this class

    //Quick fix to get rid of windows.h which contains
    //BITMAPINFOHEADER and BITMAPFILEHEADER.
    typedef unsigned short int OWORD;    //should be 16 bit
    typedef unsigned int ODWORD;         //should be 32 bit
    typedef int OLONG;                   //should be 32 bit

    struct OBITMAPINFOHEADER {
        ODWORD biSize;
        OLONG  biWidth;
        OLONG  biHeight;
        OWORD  biPlanes;
        OWORD  biBitCount;
        ODWORD biCompression;
        ODWORD biSizeImage;
        OLONG  biXPelsPerMeter;
        OLONG  biYPelsPerMeter;
        ODWORD biClrUsed;
        ODWORD biClrImportant;
    };

    struct OBITMAPFILEHEADER {
        OWORD  bfType;
        ODWORD bfSize;
        OWORD  bfReserved1;
        OWORD  bfReserved2;
        ODWORD bfOffBits;
    };
};

#endif // TEXTURE_H

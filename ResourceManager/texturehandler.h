#ifndef TEXTUREHANDLER_H
#define TEXTUREHANDLER_H

#include <QOpenGLFunctions_4_1_Core>

struct Texture
{
    int mColumns{};
    int mRows{};
    unsigned char *mBitmap{nullptr};
    ///If this is a cubemap, this will be an array of 6 mBitmaps
    unsigned char *mCubemap[6]{nullptr};

    GLuint mGLTextureID{0};          //Texture ID that OpenGL makes when glGenTextures is called

    int mBytesPrPixel{};
    bool mAlphaUsed{false};
};

class TextureHandler : protected QOpenGLFunctions_4_1_Core
{
public:
    TextureHandler();

    int makeTexture(const std::string &filename = "none", bool cubeMap = false);

//    float getHeightFromIndex(int i);

    std::map<std::string, unsigned int> mTextureMap;
    std::vector<Texture> mTextures;

private:
    int makeDefault();
    int makeBMP(const std::string &filename, bool cubeMap = false);
    int readBitmap(const std::string &filename);
    void setTexture(Texture &textureIn);
//    int readCubeMap(const std::string &filename);
//    void setCubemapTexture(Texture &textureIn);

    /*************************************/
    //this is put inside this class to avoid spamming the main namespace
    //with stuff that only is used inside this class

    //Quick fix to get rid of windows.h which contains
    //BITMAPINFOHEADER and BITMAPFILEHEADER.
    typedef unsigned short int OWORD;    //should be 16 bit
    typedef unsigned int ODWORD;         //should be 32 bit
    typedef int OLONG;                   //should be 32 bit

    struct OBITMAPINFOHEADER {
        ODWORD biSize{0};
        OLONG  biWidth{0};
        OLONG  biHeight{0};
        OWORD  biPlanes{0};
        OWORD  biBitCount{0};
        ODWORD biCompression{0};
        ODWORD biSizeImage{0};
        OLONG  biXPelsPerMeter{0};
        OLONG  biYPelsPerMeter{0};
        ODWORD biClrUsed{0};
        ODWORD biClrImportant{0};
    };

    struct OBITMAPFILEHEADER {
        OWORD  bfType{0};
        ODWORD bfSize{0};
        OWORD  bfReserved1{0};
        OWORD  bfReserved2{0};
        ODWORD bfOffBits{0};
    };
};

#endif // TEXTUREHANDLER_H

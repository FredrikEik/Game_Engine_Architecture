#ifndef SKYBOX_H
#define SKYBOX_H
#include "stb_image.h"
#include "visualobject.h"
#include <vector>
//#include "texturehandler.h"

class Skybox : public VisualObject
{
public:
    Skybox();
    ~Skybox() override;
    void makeVerticies(MeshComponent* mMesh);
    void setTexture();
   // TextureHandler *mTextures;
 //  Texture *mTextures[6]{nullptr};
    unsigned int textureID;
    std::string mTextureFilename;
    unsigned char *mBitmap{nullptr};
     unsigned char *mCubemap[6]{nullptr};
     GLuint mGLTextureID{0};
     int mColumns{};
     int mRows{};
private:
void readCubeMap();
void setCubemapTexture();
void IsCubeMap(const string &filename, bool cubeMap);
void readBitmap(const std::string &filename);
 bool mAlphaUsed{false};
 int mBytesPrPixel{};

 //    //this is put inside this class to avoid spamming the main namespace
 //    //with stuff that only is used inside this class

 //    //Quick fix to get rid of windows.h which contains
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
//    std::string SkyboxName = "Skyb1";
//    std::string mFaces[6]
//    {
//        "skybox/Skyb1.bmp",
//        "skybox/Skyb2.bmp",
//        "skybox/Skyb3.bmp",
//        "skybox/Skyb4.bmp",
//        "skybox/Skyb5.bmp",
//        "skybox/Skyb6.bmp"
//    };
};

#endif // SKYBOX_H


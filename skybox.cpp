#include "skybox.h"
#include "stb_image.h"

Skybox::Skybox()
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMesh = new MeshComponent();
    mCollision = new CollisionComponent;
    makeVerticies(mMesh);
    mMaterial = new MaterialComponent();
    mNameComp = new NameComponent();
    mNameComp->ObjectName = "SkyBox";
  //  IsCubeMap("Skybb", true);

}



Skybox::~Skybox()
{

}

void Skybox::makeVerticies(MeshComponent *mMesh)
{
    float a = 50;
    mMesh->mVertices.push_back(Vertex{-a, a, -a, 0,0,0,  0.0f, 0.0});
    mMesh->mVertices.push_back(Vertex{-a, -a, -a, 0,0,0, 1.0f, 0.0f});
    mMesh->mVertices.push_back(Vertex{a, -a, -a, 0,0,0,  1.0f, 1.0f});
    mMesh->mVertices.push_back(Vertex{a, -a, -a, 0,0,0,  1.0f, 1.0f });
    mMesh->mVertices.push_back(Vertex{a,  a, -a, 0,0,0,  0.0f, 1.0f });
    mMesh->mVertices.push_back(Vertex{-a,  a, -a, 0,0,0, 0.0f, 0.0f });

    mMesh->mVertices.push_back(Vertex{-a, -a,  a, 0,0,0, 0.0f, 0.0f });
    mMesh->mVertices.push_back(Vertex{-a, -a, -a, 0,0,0, 1.0f, 0.0f });
    mMesh->mVertices.push_back(Vertex{-a,  a, -a, 0,0,0, 1.0f, 1.0f });
    mMesh->mVertices.push_back(Vertex{-a,  a, -a, 0,0,0, 1.0f, 1.0f });
    mMesh->mVertices.push_back(Vertex{-a,  a,  a, 0,0,0, 0.0f, 1.0f });
    mMesh->mVertices.push_back(Vertex{-a, -a,  a, 0,0,0, 0.0f, 0.0f });

    mMesh->mVertices.push_back(Vertex{a, -a, -a, 0,0,0, 1.0f, 0.0f  });
    mMesh->mVertices.push_back(Vertex{a, -a,  a, 0,0,0, 1.0f, 1.0f  });
    mMesh->mVertices.push_back(Vertex{a,  a,  a, 0,0,0, 0.0f, 1.0f  });
    mMesh->mVertices.push_back(Vertex{a,  a,  a, 0,0,0, 0.0f, 1.0f  });
    mMesh->mVertices.push_back(Vertex{a,  a, -a, 0,0,0, 0.0f, 0.0f  });
    mMesh->mVertices.push_back(Vertex{a, -a, -a, 0,0,0, 1.0f, 0.0f  });

    mMesh->mVertices.push_back(Vertex{ -a,-a,a, 0,0,0,  1.0f, 0.0f  });
    mMesh->mVertices.push_back(Vertex{-a,  a,a, 0,0,0,  1.0f, 1.0f  });
    mMesh->mVertices.push_back(Vertex{a,  a,  a, 0,0,0, 0.0f, 1.0f  });
    mMesh->mVertices.push_back(Vertex{ a, a, a, 0,0,0,  0.0f, 1.0f });
    mMesh->mVertices.push_back(Vertex{a, -a, a, 0,0,0,  0.0f, 0.0f });
    mMesh->mVertices.push_back(Vertex{-a,-a,a, 0,0,0,   1.0f, 0.0f });

    mMesh->mVertices.push_back(Vertex{-a,  a, -a,0,0,0, 0.0f, 1.0f });
    mMesh->mVertices.push_back(Vertex{a,  a, -a,0,0,0,  1.0f, 1.0f  });
    mMesh->mVertices.push_back(Vertex{a,  a,  a,0,0,0,  1.0f, 0.0f  });
    mMesh->mVertices.push_back(Vertex{a,  a,  a,0,0,0,  1.0f, 0.0f  });
    mMesh->mVertices.push_back(Vertex{-a,  a,  a,0,0,0, 0.0f, 0.0f });
    mMesh->mVertices.push_back(Vertex{-a,  a, -a,0,0,0, 0.0f, 1.0f });

    mMesh->mVertices.push_back(Vertex{-a, -a, -a,0,0,0, 0.0f, 1.0f });
    mMesh->mVertices.push_back(Vertex{-a, -a,  a,0,0,0, 1.0f, 1.0f });
    mMesh->mVertices.push_back(Vertex{a, -a, -a,0,0,0,  1.0f, 0.0f  });
    mMesh->mVertices.push_back(Vertex{a, -a, -a,0,0,0,  1.0f, 0.0f  });
    mMesh->mVertices.push_back(Vertex{-a, -a,  a,0,0,0, 0.0f, 0.0f });
    mMesh->mVertices.push_back(Vertex{a, -a,  a,0,0,0,  0.0f, 1.0f  });

}



void Skybox::setTexture()
{
    // mTextures->Texture::Texture("Skyb",true);

    //    initializeOpenGLFunctions();
    //    glGenTextures(1, &textureID);
    //    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);


    //    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    //    mTextures[0] = new Texture(mFaces[0]);
    //    mTextures[1] = new Texture(mFaces[1]);
    //    mTextures[2] = new Texture(mFaces[2]);
    //    mTextures[3] = new Texture(mFaces[3]);
    //    mTextures[4] = new Texture(mFaces[4]);
    //    mTextures[5] = new Texture(mFaces[5]);



    //    int columns = mTextures[0]->getColumns();
    //    int row = mTextures[0]->getRows();

    //    for(auto i = 0; i<6; i++)
    //    {
    //        glTexImage2D(
    //            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
    //            0, GL_RGBA, row, columns, 0, GL_BGRA, GL_UNSIGNED_BYTE, mTextures[i]->getMap());

    //    }
    //    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    //    glActiveTexture(GL_TEXTURE2);
    //    glBindTexture(GL_TEXTURE_CUBE_MAP,2);

}

void Skybox::IsCubeMap(const string &filename, bool cubeMap)
{
    // initializeOpenGLFunctions();
    if(cubeMap) //Skybox - environment map
    {
        mTextureFilename = filename;
        readCubeMap();
        setCubemapTexture();
    }
}

void Skybox::readBitmap(const std::string &filename)
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
            return;
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
}

void Skybox::readCubeMap()
{
    std::string justName;
    std::stringstream sStream;
    sStream << mTextureFilename;
    std::getline(sStream, justName, '.');   //deleting .bmp
    justName.pop_back();    //removing 1
    for(int i{0}; i< 6; i++)
    {
        //TODO: clean this up! Decide where CubeMaps should be located
        std::string temp = "skybox/"+justName + std::to_string(i+1) +  ".bmp";   //adding Cubemap path and 1 - 6 to filename
        readBitmap(temp);
        mCubemap[i] = mBitmap;
    }
}

void Skybox::setCubemapTexture()
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

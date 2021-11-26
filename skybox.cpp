#include "skybox.h"
#include "stb_image.h"

Skybox::Skybox(/*Texture *mTextures*/)
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMesh = new MeshComponent();
    mCollision = new CollisionComponent;
    makeVerticies(mMesh);
    mMaterial = new MaterialComponent();
    mNameComp = new NameComponent();
    mNameComp->ObjectName = "SkyBox";

}


Skybox::~Skybox()
{

}

void Skybox::makeVerticies(MeshComponent *mMesh)
{
    float a = 25;
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


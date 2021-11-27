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
    float a = 15;
    mMesh->mVertices.push_back(Vertex{-a, a, -a, 0,0,0,  0.0f, 0.0});
    mMesh->mVertices.push_back(Vertex{-a, -a, -a, 0,0,0, 1.0f, 0.0f});
    mMesh->mVertices.push_back(Vertex{a, -a, -a, 0,0,0,  1.0f, 1.0f});
    mMesh->mVertices.push_back(Vertex{a, -a, -a, 0,0,0,  1.0f, 1.0f });
    mMesh->mVertices.push_back(Vertex{a,  a, -a, 0,0,0,  0.0f, 1.0f }); // bottom surface
    mMesh->mVertices.push_back(Vertex{-a,  a, -a, 0,0,0, 0.0f, 0.0f });

    mMesh->mVertices.push_back(Vertex{-a, -a,  a, 0,0,0, 0.0f, 0.0f });
    mMesh->mVertices.push_back(Vertex{-a, -a, -a, 0,0,0, 1.0f, 0.0f });
    mMesh->mVertices.push_back(Vertex{-a,  a, -a, 0,0,0, 1.0f, 1.0f });
    mMesh->mVertices.push_back(Vertex{-a,  a, -a, 0,0,0, 1.0f, 1.0f }); // right surface
    mMesh->mVertices.push_back(Vertex{-a,  a,  a, 0,0,0, 0.0f, 1.0f });
    mMesh->mVertices.push_back(Vertex{-a, -a,  a, 0,0,0, 0.0f, 0.0f });

    mMesh->mVertices.push_back(Vertex{a, -a, -a, 0,0,0, 1.0f, 0.0f  });
    mMesh->mVertices.push_back(Vertex{a, -a,  a, 0,0,0, 1.0f, 1.0f  });
    mMesh->mVertices.push_back(Vertex{a,  a,  a, 0,0,0, 0.0f, 1.0f  }); // top surface
    mMesh->mVertices.push_back(Vertex{a,  a,  a, 0,0,0, 0.0f, 1.0f  });
    mMesh->mVertices.push_back(Vertex{a,  a, -a, 0,0,0, 0.0f, 0.0f  });
    mMesh->mVertices.push_back(Vertex{a, -a, -a, 0,0,0, 1.0f, 0.0f  });

    mMesh->mVertices.push_back(Vertex{ -a,-a,a, 0,0,0,  1.0f, 0.0f  });
    mMesh->mVertices.push_back(Vertex{-a,  a,a, 0,0,0,  1.0f, 1.0f  });
    mMesh->mVertices.push_back(Vertex{a,  a,  a, 0,0,0, 0.0f, 1.0f  });  //left surface
    mMesh->mVertices.push_back(Vertex{ a, a, a, 0,0,0,  0.0f, 1.0f });
    mMesh->mVertices.push_back(Vertex{a, -a, a, 0,0,0,  0.0f, 0.0f });
    mMesh->mVertices.push_back(Vertex{-a,-a,a, 0,0,0,   1.0f, 0.0f });

    mMesh->mVertices.push_back(Vertex{-a,  a, -a,0,0,0, 0.0f, 1.0f });
    mMesh->mVertices.push_back(Vertex{a,  a, -a,0,0,0,  1.0f, 1.0f  });
    mMesh->mVertices.push_back(Vertex{a,  a,  a,0,0,0,  1.0f, 0.0f  }); // back surface
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






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
    mNameComp->mName = "SkyBox";
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

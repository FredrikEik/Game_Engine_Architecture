#include "player.h"

Player::Player()
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMesh = new MeshComponent();
    mMesh->mDrawType = GL_TRIANGLES;
    mMaterial = new MaterialComponent();

    mMesh->mVertices.push_back(Vertex{0,0,0, 1,0,0});
    mMesh->mVertices.push_back(Vertex{0.5,0,0, 1,0,0});       // bottom surface
    mMesh->mVertices.push_back(Vertex{0,0.5,0, 1,0,0});

    mMesh->mVertices.push_back(Vertex{0.5,0,0,  1,0,0});
    mMesh->mVertices.push_back(Vertex{0.5,0.5,0,  1,0,0});
    mMesh->mVertices.push_back(Vertex{0,0.5,0,  1,0,0});

    mMesh->mVertices.push_back(Vertex{0.5,0,0,  1,0,1});
    mMesh->mVertices.push_back(Vertex{0.5,0.5,0,  1,0,1});       // right surface
    mMesh->mVertices.push_back(Vertex{0.5,0.5,0.5,  1,0,1});

    mMesh->mVertices.push_back(Vertex{0.5,0,0,  1,0,1});
    mMesh->mVertices.push_back(Vertex{0.5,0.5,0.5,  1,0,1});
    mMesh->mVertices.push_back(Vertex{0.5,0,0.5,  1,0,1});

    mMesh->mVertices.push_back(Vertex{0,0,0.5,  1,1,0});
    mMesh->mVertices.push_back(Vertex{0,0.5,0.5,  1,1,0});       // top surface
    mMesh->mVertices.push_back(Vertex{0.5,0.5,0.5,  1,1,0});
    mMesh->mVertices.push_back(Vertex{0,0,0.5,  1,1,0});
    mMesh->mVertices.push_back(Vertex{0.5,0,0.5,  1,1,0});
    mMesh->mVertices.push_back(Vertex{0.5,0.5,0.5,  1,1,0});

    mMesh->mVertices.push_back(Vertex{0,0,0.5,  1,0,0});
    mMesh->mVertices.push_back(Vertex{0,0,0,  1,0,0});       //left surface
    mMesh->mVertices.push_back(Vertex{0,0.5,0.5,  1,0,0});

    mMesh->mVertices.push_back(Vertex{0,0,0,  1,0,0});
    mMesh->mVertices.push_back(Vertex{0,0.5,0,  1,0,0});
    mMesh->mVertices.push_back(Vertex{0,0.5,0.5,  1,0,0});

    mMesh->mVertices.push_back(Vertex{0,0.5,0,  1,0,1});
    mMesh->mVertices.push_back(Vertex{0,0.5,0.5,  1,0,1});   // back surface
    mMesh->mVertices.push_back(Vertex{0.5,0.5,0,  1,0,1});

    mMesh->mVertices.push_back(Vertex{0.5,0.5,0,  1,0,1});
    mMesh->mVertices.push_back(Vertex{0.5,0.5,0.5,  1,0,1});
    mMesh->mVertices.push_back(Vertex{0,0.5,0.5,  1,0,1});
}

void Player::move(float dx, float dy, float dz)
{
    mTransform->mMatrix.translate(dx, dy, dz);
}

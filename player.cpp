#include "player.h"

Player::Player(std::string filename)
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMesh = new MeshComponent();
    readFile(filename);
    mMesh->mDrawType = GL_TRIANGLES;
    mMaterial = new MaterialComponent();

}



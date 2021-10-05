#include "player.h"

Player::Player() : mx{0.0f}, my{0.0f}, mz{0.0f}
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mTransform->mPosition = gsl::Vector3D(mx,my,mz);
    mMesh = new MeshComponent();
    mCollision = new CollisionSystem;
    readFile("../GEA2021/Assets/Pacman.obj");
    mCollision->setBoundingBox(gsl::Vector3D(-1, -1, -1),gsl::Vector3D(1, 1, 1));
    mCollision->setBoundingSphere(1, mTransform->mPosition);
    mMaterial = new MaterialComponent();
}

Player::~Player()
{

}

void Player::move(float dx, float dy, float dz)
{
    mTransform->mPosition += gsl::Vector3D(dx,dy,dz);
    mTransform->mMatrix.translate(dx,dy,dz);
    mCollision->moveBoundingBox(dx, dy, dz);
    mCollision->moveBoundingSphere(dx, dy, dz);
}

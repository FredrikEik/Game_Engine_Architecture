#include "player.h"

Player::Player() : mx{0.0f}, my{0.0f}, mz{0.0f}
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mTransform->mPosition = gsl::Vector3D(mx,my,mz);
    mMesh = new MeshComponent();
    mCollision = new CollisionComponent;
    mColSystem = new CollisionSystem;
    //factoryPtr->createShape("Pacman.obj");
    readFile("../GEA2021/Assets/Pacman.obj");
    mCollision->setBoundingBox(gsl::Vector3D(-1, -1, -1),gsl::Vector3D(1, 1, 1));
    mCollision->setBoundingSphere(1, mTransform->mPosition);
    mMaterial = new MaterialComponent();
    mInputComp = new InputComponent;
    mNameComp = new NameComponent();
    mNameComp->mName = "Player";
    mNameComp->objectID = 8;


}

Player::~Player()
{

}

void Player::move(float dx, float dy, float dz)
{
    mTransform->mPosition += gsl::Vector3D(dx,dy,dz);
    mTransform->mMatrix.translate(dx,dy,dz);
    mColSystem->moveBoundingBox(dx, dy, dz,mCollision);
    mColSystem->moveBoundingSphere(dx, dy, dz,mCollision);
}

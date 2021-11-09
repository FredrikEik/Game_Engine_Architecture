#include "player.h"

Player::Player(ShapeFactory* f) : mx{0.0f}, my{0.0f}, mz{0.0f}
{
    factoryPtr = f;
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mTransform->mPosition = gsl::Vector3D(mx,my,mz);
    mMesh = factoryPtr->getMesh(4);
    mCollision = factoryPtr->getColli(4);
    mColSystem = new CollisionSystem;
    mCollision->setBoundingSphere(1, mTransform->mPosition);
    mMaterial = new MaterialComponent();
    mInputComp = new InputComponent;
    mNameComp = new NameComponent();
    mNameComp->mName = "Player";
    mNameComp->objectID = 5;
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

#include "player.h"

Player::Player(ShapeFactory* f)
{
    factoryPtr = f;
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMoveComp = new MovementComponent;
    mTransform->mScale = 0.5;
    mMesh = factoryPtr->getMesh(4);
    mCollision = factoryPtr->getColli(4);
    mTransform->mMatrix.scale(mTransform->mScale);
    mColSystem = new CollisionSystem;
    mCollision->setBoundingSphere(0.5, mTransform->mPosition);
    mMaterial = new MaterialComponent();

    mNameComp = new NameComponent();
    mNameComp->ObjectName = "Player";
    mNameComp->ObjectID = 0;
}

Player::~Player()
{

}



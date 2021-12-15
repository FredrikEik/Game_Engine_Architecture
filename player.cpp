#include "player.h"

Player::Player(ShapeFactory* f)
{
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMoveComp = new MovementComponent;
    mTransform->mScale = 0.5;
    mMesh = f->getMesh(4);
    mCollision = f->getColli(4);
    mTransform->mMatrix.scale(mTransform->mScale);
    mCollision->setBoundingSphere(0.5, mTransform->mPosition);
    mMaterial = new MaterialComponent();

    mNameComp = new NameComponent();
    mNameComp->mName = "Player";
    mNameComp->objectID = 0;
}

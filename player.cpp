#include "player.h"

Player::Player(ShapeFactory* f) : mx{0.0f}, my{0.0f}, mz{0.0f}
{
    factoryPtr = f;
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mMoveComp = new MovementComponent;
    mTransform->mScale = 0.5;
    mTransform->mPosition = gsl::Vector3D(mx,my,mz);
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


void Player::centerPlayer()
{
    gsl::Vector3D mPos = mTransform->mPosition;
    gsl::Vector3D mRound {std::round(mTransform->mPosition.x), 0, std::round(mTransform->mPosition.z)};

    float xDiff = mRound.x - mPos.x;
    float zDiff = mRound.z - mPos.z;
    gsl::Vector3D temp(xDiff,0,zDiff);


    mTransform->mPosition += temp;
    mTransform->mMatrix.setPosition(mTransform->mPosition.x, mTransform->mPosition.y, mTransform->mPosition.z);
    mColSystem->moveBoundingBox(temp.x, temp.y, temp.z, mCollision);
    mColSystem->moveBoundingSphere(temp.x, temp.y, temp.z, mCollision);
}


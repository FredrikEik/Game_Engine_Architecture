#include "player.h"

Player::Player(ShapeFactory* f) : mx{0.0f}, my{0.0f}, mz{0.0f}
{
    factoryPtr = f;
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mTransform->mPosition = gsl::Vector3D(mx,my,mz);
    mMesh = factoryPtr->getMesh(4);
    mCollision = factoryPtr->getColli(4);
    mTransform->mMatrix.scale(0.5);
    mColSystem = new CollisionSystem;
    mCollision->setBoundingSphere(0.5, mTransform->mPosition);
    mMaterial = new MaterialComponent();
    mMoveComp = new MovementComponent;
    mNameComp = new NameComponent();
    mNameComp->mName = "Player";
    mNameComp->objectID = 0;
}

Player::~Player()
{

}

void Player::centerPlayer()
{
    gsl::Vector3D mPos = mTransform->mPosition;
    gsl::Vector3D mRound {std::round(mPos.x), 0, std::round(mPos.z)};

    float xDiff = mRound.x - mPos.x;
    float zDiff = mRound.z - mPos.z;
    gsl::Vector3D temp(xDiff,0,zDiff);

    //qDebug() << xDiff << zDiff;

    mTransform->mPosition += temp;
    mTransform->mMatrix.setPosition(mTransform->mPosition.x, mTransform->mPosition.y, mTransform->mPosition.z);
    mColSystem->moveBoundingBox(temp.x, temp.y, temp.z, mCollision);
    mColSystem->moveBoundingSphere(temp.x, temp.y, temp.z, mCollision);
}

void Player::movePlayer()
{
    move(mForward.x * mMoveComp->mPlayerSpeed, mForward.y * mMoveComp->mPlayerSpeed, mForward.z * mMoveComp->mPlayerSpeed);
}

void Player::setForward(float dx, float dz)
{
    mForward = {dx, 0, dz};
    mTransform->mMatrix.setRotationToVector(mForward);
    mTransform->mMatrix.scale(0.5);
}

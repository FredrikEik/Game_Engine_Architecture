#include "player.h"

Player::Player(ShapeFactory* f) : mx{0.0f}, my{0.0f}, mz{0.0f}
{
    factoryPtr = f;
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mTransform->mPosition = gsl::Vector3D(mx,my,mz);
    mMesh = factoryPtr->getMesh(5);
    mCollision = factoryPtr->getColli(5);
    mTransform->mMatrix.scale(0.3);
    mColSystem = new CollisionSystem;
    mCollision->setBoundingSphere(0.3, mTransform->mPosition);
    mMaterial = new MaterialComponent();
    mInputComp = new InputComponent;
    mNameComp = new NameComponent();
    mNameComp->ObjectName = "Player";
    mNameComp->ObjectID = 0;
}

Player::~Player()
{

}

//void Player::move(float dx, float dy, float dz)
//{
//    gsl::Vector3D temp(0,0,0);
//    if(dz > 0 && onBwallY == true)
//        temp.z = 0;
//    else if(dz < 0 && onFwallY == true)
//        temp.z = 0;
//    else
//        temp.z = dz;

//    if(dx > 0 && onLwallX == true)
//        temp.x = 0;
//    else if(dx < 0 && onRwallX == true)
//        temp.x = 0;
//    else
//        temp.x = dx;

//    temp.y = dy;

//    mTransform->mPosition += temp;
//    mTransform->mMatrix.setPosition(mTransform->mPosition.x, mTransform->mPosition.y, mTransform->mPosition.z);
//    mColSystem->moveBoundingBox(temp.x, temp.y, temp.z, mCollision);
//    mColSystem->moveBoundingSphere(temp.x, temp.y, temp.z, mCollision);
//}

void Player::rotateBackwardV()
{
    gsl::Vector3D temp{0,0,0};
    if(mBackward.x == -1)
        temp = gsl::Vector3D(1,0,0);
    else if(mBackward.x == 1)
        temp = gsl::Vector3D(-1,0,0);
    else if(mBackward.z == -1)
        temp = gsl::Vector3D(0,0,1);
    else if(mBackward.z == 1)
        temp = gsl::Vector3D(0,0,-1);
    else
        qDebug() << "Error rotating Player Backward vector";

    mBackward = temp;
}


void Player::centerPlayer()
{
    gsl::Vector3D mPos = mTransform->mPosition;
    gsl::Vector3D mRound {std::round(mTransform->mPosition.x), 0, std::round(mTransform->mPosition.z)};

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
    move(mForward.x * mInputComp->mPlayerSpeed, mForward.y * mInputComp->mPlayerSpeed, mForward.z * mInputComp->mPlayerSpeed);
}

void Player::setForward(float dx, float dz)
{
    mForward = {dx, 0, dz};
    mTransform->mMatrix.setRotationToVector(mForward);
    mTransform->mMatrix.scale(0.5);
}

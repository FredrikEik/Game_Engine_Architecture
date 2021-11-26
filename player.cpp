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
    mInputComp = new InputComponent;
    mNameComp = new NameComponent();
    mNameComp->mName = "Player";
    mNameComp->objectID = 0;
}

Player::~Player()
{

}

//void Player::move(float dx, float dy, float dz)
//{
//    gsl::Vector3D mPos = mTransform->mPosition;
//    gsl::Vector3D mRound {std::round(mTransform->mPosition.x), 0, std::round(mTransform->mPosition.z)};

//    float xDiff = mRound.x - mPos.x;
//    float zDiff = mRound.z - mPos.z;
//    gsl::Vector3D temp(0,0,0);
//    if(dz > 0 && onBwallY == true){
//        temp.z = zDiff; temp.x = xDiff;}
//    else if(dz < 0 && onFwallY == true){
//        temp.z = zDiff; temp.x = xDiff;}
//    else{
//        temp.z = dz;}

//    if(dx > 0 && onLwallX == true){
//        temp.x = xDiff; temp.z = zDiff;}
//    else if(dx < 0 && onRwallX == true){
//        temp.x = xDiff; temp.z = zDiff;}
//    else{
//        temp.x = dx;}

//    temp.y = dy;

//    qDebug() << xDiff << zDiff;

//    mTransform->mPosition += temp;
//    mTransform->mMatrix.setPosition(mTransform->mPosition.x, mTransform->mPosition.y, mTransform->mPosition.z);
//    mColSystem->moveBoundingBox(temp.x, temp.y, temp.z, mCollision);
//    mColSystem->moveBoundingSphere(temp.x, temp.y, temp.z, mCollision);
//}

void Player::move(float dx, float dy, float dz)
{
    gsl::Vector3D mPos = mTransform->mPosition;
    gsl::Vector3D mRound {std::round(mTransform->mPosition.x), 0, std::round(mTransform->mPosition.z)};

    float xDiff = mRound.x - mPos.x;
    float zDiff = mRound.z - mPos.z;
    gsl::Vector3D temp(0,0,0);
    if(dz > 0 && onBwallY == true){
        temp.z = zDiff; temp.x = xDiff;}
    else if(dz < 0 && onFwallY == true){
        temp.z = zDiff; temp.x = xDiff;}
    else{
        temp.z = dz;}

    if(dx > 0 && onLwallX == true){
        temp.x = xDiff; temp.z = zDiff;}
    else if(dx < 0 && onRwallX == true){
        temp.x = xDiff; temp.z = zDiff;}
    else{
        temp.x = dx;}

    temp.y = dy;

    qDebug() << xDiff << zDiff;

    mTransform->mPosition += temp;
    mTransform->mMatrix.setPosition(mTransform->mPosition.x, mTransform->mPosition.y, mTransform->mPosition.z);
    mColSystem->moveBoundingBox(temp.x, temp.y, temp.z, mCollision);
    mColSystem->moveBoundingSphere(temp.x, temp.y, temp.z, mCollision);
}

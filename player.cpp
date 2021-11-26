#include "player.h"

Player::Player(ShapeFactory* f) : mx{0.0f}, my{0.0f}, mz{0.0f}
{
    factoryPtr = f;
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mTransform->mPosition = gsl::Vector3D(mx,my,mz);
    mMesh = factoryPtr->getMesh(4);
    mCollision = factoryPtr->getColli(4);
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

void Player::move(float dx, float dy, float dz)
{
    gsl::Vector3D temp(0,0,0);
    if(dz > 0 && onBwallY == true)
        temp.z = 0;
    else if(dz < 0 && onFwallY == true)
        temp.z = 0;
    else
        temp.z = dz;

    if(dx > 0 && onLwallX == true)
        temp.x = 0;
    else if(dx < 0 && onRwallX == true)
        temp.x = 0;
    else
        temp.x = dx;

    temp.y = dy;

    mTransform->mPosition += temp;
    mTransform->mMatrix.setPosition(mTransform->mPosition.x, mTransform->mPosition.y, mTransform->mPosition.z);
    mColSystem->moveBoundingBox(temp.x, temp.y, temp.z, mCollision);
    mColSystem->moveBoundingSphere(temp.x, temp.y, temp.z, mCollision);
}


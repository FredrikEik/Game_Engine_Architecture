#include "player.h"

Player::Player(ShapeFactory* f) : mx{0.0f}, my{0.0f}, mz{0.0f}
{
    factoryPtr = f;
    mTransform = new TransformComponent();
    mTransform->mMatrix.setToIdentity();
    mTransform->mPosition = gsl::Vector3D(mx,my,mz);
    mMesh = factoryPtr->getMesh(4);
    mCollision = factoryPtr->getColli(4);
    //mTransform->mMatrix.scale(0.5);
    mColSystem = new CollisionSystem;
    mCollision->setBoundingSphere(1, mTransform->mPosition);
    mMaterial = new MaterialComponent();
    mInputComp = new InputComponent;
    mNameComp = new NameComponent();
    mNameComp->mName = "Player";
    mNameComp->objectID = 0;
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
    mTransform->mMatrix.translate(temp.x, temp.y, temp.z);
    mColSystem->moveBoundingBox(temp.x, temp.y, temp.z, mCollision);
    mColSystem->moveBoundingSphere(temp.x, temp.y, temp.z, mCollision);
}

void Player::CheckPlayerWall(CollisionComponent *bCollision)
{
    auto x = fmax(bCollision->BoundingBoxMin.getX(), fmin(mCollision->center.getX(), bCollision->BoundingBoxMax.getX()));
    auto z = fmax(bCollision->BoundingBoxMin.getZ(), fmin(mCollision->center.getZ(), bCollision->BoundingBoxMax.getZ()));
    auto xdiff = x - mCollision->center.getX();
    auto zdiff = z - mCollision->center.getZ();

    //qDebug() << mCollision->center;
    auto distance = sqrt((x - mCollision->center.getX()) * (x - mCollision->center.getX()));
    if(distance < mCollision->radius)
    {
        if(xdiff > 0.f)
            onLwallX = true;//qDebug() << "onLwallX";
        if(xdiff < 0.f)
            onRwallX = true;//qDebug() << "onRwallX";
    }
    else if(distance >= mCollision->radius)
    {
        onRwallX = false;onLwallX = false;
    }
    distance = sqrt((z - mCollision->center.getZ()) * (z - mCollision->center.getZ()));
    if(distance < mCollision->radius)
    {
        if(zdiff > 0.f)
            onBwallY = true;//qDebug() << "onBwallY";
        if(zdiff < 0.f)
            onFwallY = true;//qDebug() << "onFwallY";
    }
    else if(distance >= mCollision->radius)
    {
        onBwallY = false;onFwallY = false;
    }
}

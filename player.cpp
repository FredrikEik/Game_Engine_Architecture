#include "player.h"

player::player(GameObject* mesh)
{
    mMesh = mesh;
}

player::~player()
{

}

void player::Move(float dt)
{
    if (mMesh)
    {
        gsl::Vector3D position = mMesh->getTransformComp()->mTrueScaleMatrix.getPosition();
        if (position.x > 5.f && dt > 0.f)
        {
            //qDebug() << "x: " << position.x << " dt: " << dt;
            return;
        }
        else if (position.x < 1.f && dt < 0.f)
        {
            //qDebug() << "x: " << position.x << " dt: " << dt;
            return;
        }
        mMesh->getTransformComp()->mTrueScaleMatrix.setPosition(position.getX() + dt, position.getY(), position.getZ());
        mMesh->getTransformComp()->mMatrix.setPosition(position.getX() + dt, position.getY(), position.getZ());
    }
}

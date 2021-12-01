#include "gameobject.h"


void GameObject::move(float x, float y, float z)
{
    mTransform->mMatrix.translate(x, y, z);
}

//void GameObject::setSurface(GameObject *surface)
//{
//    _las = surface;
//    surfVertices = _las->getmVertices();
//    int mT = static_cast<int>(surfVertices.size());
//    if(mMeshHandlerGameObject->surfVertices.size()>100){
//        mT = rand()%mT;
//        qDebug() << mT;
//        gsl::Vector3D v1 = surfVertices.at(mT).get_xyz();
//        gsl::Vector3D v2 = surfVertices.at(mT+1).get_xyz();
//        gsl::Vector3D v3 = surfVertices.at(mT+2).get_xyz();
//        gsl::Vector3D pos = (v1+v2+v3)*0.333;
//        pos.z += 50;
//        mTransform->mMatrix.setPosition(pos.x, pos.y, pos.z);}
//    else
//        move(1,1,5);
//}

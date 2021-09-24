#include "components.h"
#include "gsl/matrix4x4.h"

//Components::Components()
//{

//}

TransformComponent::TransformComponent()
{

}

MeshComponent::MeshComponent()
{

}

MaterialComponent::MaterialComponent()
{

}

CollisionComponent::CollisionComponent()
{
    /*QMatrix4x4 collisionAsMatrix;
    QVector4D answer;

    //CP1 to worldspace
    collisionAsMatrix.setToIdentity();
    collisionAsMatrix.setColumn(3, QVector4D(cpxOneLocal, 0.0f, cpyOneLocal, 1.0f));
    answer = (mMatrix * collisionAsMatrix).column(3);
    cpxOne = answer.x();
    cpyOne = answer.z();

    //CP2 to worldspace
    collisionAsMatrix.setToIdentity();
    collisionAsMatrix.setColumn(3, QVector4D(cpxTwoLocal, 0.0f, cpyTwoLocal, 1.0f));
    answer = (mMatrix * collisionAsMatrix).column(3);
    cpxTwo = answer.x();
    cpyTwo = answer.z();*/

}

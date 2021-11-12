//#include "enemy.h"

//Enemy::Enemy()
//{
//    mTransform = new TransformComponent();
//    mTransform->mMatrix.setToIdentity();
//    mMesh = new MeshComponent();
//   // mCollision = new CollisionComponent;
//    mColSystem = new CollisionSystem;
//    readFile("../GEA2021/Assets/Enemy.obj");
//    mCollision->setBoundingSphere(0.2, mTransform->mPosition);
//    mMaterial = new MaterialComponent();
//    mNameComp = new NameComponent();
//    mNameComp->ObjectName = "Enemy";

//}


//Enemy::~Enemy()
//{


//}
//void Enemy::move(Player *mPlayer, gsl::Vector3D Ppos)
//{
//    //    if(Checkmove == false)
//    //    {
//    Ppos = mPlayer->mTransform->mPosition;
//    gsl::Vector3D Epos = mTransform->mPosition;

//    float xDistance = Ppos.x-Epos.x;
//    float yDistance = Ppos.y-Epos.y;

//    float hypo = sqrt((xDistance * xDistance) + (yDistance * yDistance));


//    dir = Ppos - Epos;
//    dir.normalize();

//    dir = dir*speed;
//    qDebug() << hypo;

//    if(Checkmove == false)
//    {
//         mTransform->mMatrix.translate(dir);
//         mColsystem->moveBoundingSphere(dir.x,dir.y,dir.z,mCollision);
//    }

//}

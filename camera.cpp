#include "camera.h"
#include <QDebug>

Camera::Camera()
{
    mViewMatrix.setToIdentity();
    mProjectionMatrix.setToIdentity();

    mYawMatrix.setToIdentity();
    mPitchMatrix.setToIdentity();

    mInputComp = new InputComponent;
}

Camera::~Camera()
{
}

void Camera::pitch(float degrees)
{
    //  rotate around mRight
    mPitch -= degrees;
    updateForwardVector();
}

void Camera::yaw(float degrees)
{
    // rotate around mUp
    mYaw -= degrees;
    updateForwardVector();
}

void Camera::updateRightVector()
{
    mRight = mForward^mUp;
    mRight.normalize();
    //    qDebug() << "Right " << mRight;
}

void Camera::updateForwardVector()
{
    mRight = gsl::Vector3D(1.f, 0.f, 0.f);
    mRight.rotateY(mYaw);
    mRight.normalize();
    mUp = gsl::Vector3D(0.f, 1.f, 0.f);
    mUp.rotateX(mPitch);
    mUp.normalize();
    mForward = mUp^mRight;

    updateRightVector();
}

void Camera::update()
{
    mYawMatrix.setToIdentity();
    mPitchMatrix.setToIdentity();

    mPitchMatrix.rotateX(mPitch);
    mYawMatrix.rotateY(mYaw);

    mPosition -= mForward * mSpeed;

    mViewMatrix = mPitchMatrix* mYawMatrix;
    mViewMatrix.translate(-mPosition);
}

void FrustumSystem::updateFrustumPos(gsl::Vector3D cameraPos)
{
    move(cameraPos.x, cameraPos.y, cameraPos.z);
    mFarPlane.x  = tan(mFrustum.mFOV)*mFrustum.mFarPlaneDistance;
    mFarPlane.y  = (tan(mFrustum.mFOV)*mFrustum.mFarPlaneDistance)/mFrustum.mAspectRatio;
    mFarPlane.z  = mFrustum.mFarPlaneDistance;
    mNearPlane.x = tan(mFrustum.mFOV)*mFrustum.mNearPlaneDistance;
    mNearPlane.y = (tan(mFrustum.mFOV)*mFrustum.mNearPlaneDistance)/mFrustum.mAspectRatio;
    mNearPlane.z = mFrustum.mNearPlaneDistance;

    mRightTopFar     = gsl::Vector3D(mFarPlane.x, mFarPlane.y, -mFarPlane.z);
    mRightBotFar  = gsl::Vector3D(mFarPlane.x, -mFarPlane.y, -mFarPlane.z);
    mLeftTopFar      = gsl::Vector3D(-mFarPlane.x, mFarPlane.y, -mFarPlane.z);
    mLeftBotFar   = gsl::Vector3D(-mFarPlane.x, -mFarPlane.y, -mFarPlane.z);

    mRightTopNear    = gsl::Vector3D(mNearPlane.x, mNearPlane.y, -mNearPlane.z);
    mRightBotNear = gsl::Vector3D(mNearPlane.x, -mNearPlane.y, -mNearPlane.z);
    mLeftTopNear     = gsl::Vector3D(-mNearPlane.x, mNearPlane.y, -mNearPlane.z);
    mLeftBotNear  = gsl::Vector3D(-mNearPlane.x, -mNearPlane.y, -mNearPlane.z);
}

void Camera::calculateProjectionMatrix()
{
    mProjectionMatrix.perspective(mFrustumComp.mFOV, mFrustumComp.mAspectRatio, mFrustumComp.mNearPlaneDistance, mFrustumComp.mFarPlaneDistance);
}

void Camera::setPosition(const gsl::Vector3D &position)
{
    mPosition = position;
}

void Camera::setSpeed(float speed)
{
    mSpeed = speed;
}

void Camera::updateHeigth(float deltaHeigth)
{
    mPosition.y += deltaHeigth;
}

void Camera::moveRight(float delta)
{
    //This fixes a bug in the up and right calculations
    //so camera always holds its height when straifing
    //should be fixed thru correct right calculations!
    gsl::Vector3D right = mRight;
    right.y = 0.f;
    mPosition += right * delta;
}

gsl::Vector3D Camera::position() const
{
    return mPosition;
}

gsl::Vector3D Camera::up() const
{
    return mUp;
}

gsl::Vector3D Camera::right() const
{
    return mRight;
}

FrustumSystem::FrustumSystem()
{
    mTransform = new TransformComponent;
    mTransform->mMatrix.setToIdentity();
    mMesh = new MeshComponent;
    mCollision = new CollisionComponent;

    calculateFrustumVectors();
    makeFrustumLines();
    mCollision->setBoundingSphere(0.01, mTransform->mPosition);

    mMesh->mDrawType = GL_LINES;
    mMaterial = new MaterialComponent;
}

FrustumSystem::~FrustumSystem()
{

}

void FrustumSystem::calculateFrustumVectors()
{
    gsl::Vector3D tempVector;

    //nearplane vector = using pythagoras
    tempVector.setX(tan(mFrustum.mFOV)*mFrustum.mNearPlaneDistance); //horisontal opposite
    tempVector.setY((tan(mFrustum.mFOV)*mFrustum.mNearPlaneDistance)/mFrustum.mAspectRatio); //horisontal opposite/aspect ratio
    tempVector.setZ(-mFrustum.mNearPlaneDistance); //nearplane distance
    mNearPlane = tempVector;

    //farplane vector = using pythagoras
    tempVector.setX(tan(mFrustum.mFOV)*mFrustum.mFarPlaneDistance); //horisontal opposite
    tempVector.setY((tan(mFrustum.mFOV)*mFrustum.mFarPlaneDistance)/mFrustum.mAspectRatio); //horisontal opposite/aspect ratio
    tempVector.setZ(-mFrustum.mFarPlaneDistance); //farplane distance
    mFarPlane = tempVector;

    mRightTopNear=(gsl::Vector3D(mNearPlane.x , mNearPlane.y,  mNearPlane.z));
    mRightTopFar =(gsl::Vector3D(mFarPlane.x  , mFarPlane.y,   mFarPlane.z));
    mRightBotNear=(gsl::Vector3D(mNearPlane.x , -mNearPlane.y, mNearPlane.z));
    mRightBotFar =(gsl::Vector3D(mFarPlane.x  , -mFarPlane.y,  mFarPlane.z));
    mLeftTopNear =(gsl::Vector3D(-mNearPlane.x, mNearPlane.y,  mNearPlane.z));
    mLeftTopFar  =(gsl::Vector3D(-mFarPlane.x , mFarPlane.y,   mFarPlane.z));
    mLeftBotNear =(gsl::Vector3D(-mNearPlane.x, -mNearPlane.y, mNearPlane.z));
    mLeftBotFar  =(gsl::Vector3D(-mFarPlane.x , -mFarPlane.y,  mFarPlane.z));
}

void FrustumSystem::makeFrustumLines()
{
    gsl::Vector3D pink(1.f, 0.f, 1.f);
    gsl::Vector2D uv(0,0);
    //TOP
    mMesh->mVertices.push_back(Vertex{mRightTopNear, pink, uv});
    mMesh->mVertices.push_back(Vertex{mLeftTopNear,  pink, uv});
    mMesh->mVertices.push_back(Vertex{mRightTopNear, pink, uv});
    mMesh->mVertices.push_back(Vertex{mRightTopFar,  pink, uv});
    mMesh->mVertices.push_back(Vertex{mRightTopFar,  pink, uv});
    mMesh->mVertices.push_back(Vertex{mLeftTopFar,   pink, uv});
    mMesh->mVertices.push_back(Vertex{mLeftTopNear,  pink, uv});
    mMesh->mVertices.push_back(Vertex{mLeftTopFar,   pink, uv});

    //BOTTOM
    mMesh->mVertices.push_back(Vertex{mRightBotNear, pink, uv});
    mMesh->mVertices.push_back(Vertex{mLeftBotNear,  pink, uv});
    mMesh->mVertices.push_back(Vertex{mRightBotNear, pink, uv});
    mMesh->mVertices.push_back(Vertex{mRightBotFar,  pink, uv});
    mMesh->mVertices.push_back(Vertex{mRightBotFar,  pink, uv});
    mMesh->mVertices.push_back(Vertex{mLeftBotFar,   pink, uv});
    mMesh->mVertices.push_back(Vertex{mLeftBotNear,  pink, uv});
    mMesh->mVertices.push_back(Vertex{mLeftBotFar,   pink, uv});

    //RIGHT
    mMesh->mVertices.push_back(Vertex{mRightTopNear, pink, uv});
    mMesh->mVertices.push_back(Vertex{mRightBotNear, pink, uv});
    mMesh->mVertices.push_back(Vertex{mRightTopFar,  pink, uv});
    mMesh->mVertices.push_back(Vertex{mRightBotFar,  pink, uv});

    //LEFT
    mMesh->mVertices.push_back(Vertex{mLeftTopNear,  pink, uv});
    mMesh->mVertices.push_back(Vertex{mLeftBotNear,  pink, uv});
    mMesh->mVertices.push_back(Vertex{mLeftTopFar,   pink, uv});
    mMesh->mVertices.push_back(Vertex{mLeftBotFar,   pink, uv});
}

//bool FrustumSystem::insideFrustum(gsl::Vector3D pos)
//{
//    gsl::Vector3D U = mRightBotFar - mRightBotNear;
//    gsl::Vector3D V = mRightTopFar - mRightBotNear;

//    gsl::Vector3D Right_normal = U ^ V;

//    gsl::Vector3D xRBN = pos - mRightBotNear;

//    //double distancePastFrontPlane = xRBN * Right_normal;
//    if(xRBN * Right_normal < 0)
//    {
//        return false;
//    }

//}


void FrustumSystem::init()
{

    initializeOpenGLFunctions();


    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mMesh->mVAO );
    glBindVertexArray( mMesh->mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mMesh->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mMesh->mVBO );

    glBufferData( GL_ARRAY_BUFFER, mMesh->mVertices.size()*sizeof( Vertex ),
                  mMesh->mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mMesh->mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}



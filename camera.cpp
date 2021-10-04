#include "camera.h"
#include <QDebug>


Camera::Camera(float fieldOfView, float aspectRatio)
{
    mViewMatrix.setToIdentity();
    mProjectionMatrix.setToIdentity();

    mYawMatrix.setToIdentity();
    mPitchMatrix.setToIdentity();

    meshComp = new MeshComponent();
    transformComp = new TransformComponent();
    materialComp = new MaterialComponent();
    frustumComp = new FrustumCollisionComponent();
    frustumComp->farPlaneLength = 100.0f;
    frustumComp->nearPlaneLength = 10.0f;

    farplaneX  = tan(fieldOfView)*frustumComp->farPlaneLength;
    farplaneY  = (tan(fieldOfView)*frustumComp->farPlaneLength)/aspectRatio;
    farplaneZ  = frustumComp->farPlaneLength;
    nearplaneX = tan(fieldOfView)*frustumComp->nearPlaneLength;
    nearplaneY = (tan(fieldOfView)*frustumComp->nearPlaneLength)/aspectRatio;
    nearplaneZ = frustumComp->nearPlaneLength;

    //Camerapos
    getMeshComponent()->mVertices.push_back(Vertex{0.0f, 0.0f,  0.0f,    0.3f, 0.0f, 0.5f,    0.0f, 0.0f});
    //NearPlane
    //BottomRight
    getMeshComponent()->mVertices.push_back(Vertex{nearplaneX, -nearplaneY, nearplaneZ,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});
    //TopRight
    getMeshComponent()->mVertices.push_back(Vertex{nearplaneX, nearplaneY, nearplaneZ,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});
    //TopLeft
    getMeshComponent()->mVertices.push_back(Vertex{-nearplaneX, nearplaneY, nearplaneZ,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});
    //BottomLeft
    getMeshComponent()->mVertices.push_back(Vertex{-nearplaneX, -nearplaneY, nearplaneZ,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});
    //Farplane
    //BottomLeft
    getMeshComponent()->mVertices.push_back(Vertex{-farplaneX, -farplaneY, farplaneZ,    0.7f, 0.0f, 0.3f,    1.0f, 1.0f});
    //TopLeft
    getMeshComponent()->mVertices.push_back(Vertex{-farplaneX, farplaneY, farplaneZ,    0.5f, 0.2f, 0.6f,    0.0f, 1.0f});
    //TopRight
    getMeshComponent()->mVertices.push_back(Vertex{farplaneX, farplaneY, farplaneZ,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});
    //BottomRight
    getMeshComponent()->mVertices.push_back(Vertex{farplaneX, -farplaneY, farplaneZ,    0.5f, 0.2f, 0.6f,    1.0f, 0.0f});
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

void Camera::updateFrustumPos(float fieldOfView, float aspectRatio)
{
    farplaneX  = tan(fieldOfView)*frustumComp->farPlaneLength;
    farplaneY  = (tan(fieldOfView)*frustumComp->farPlaneLength)/aspectRatio;
    farplaneZ  = frustumComp->farPlaneLength;
    nearplaneX = tan(fieldOfView)*frustumComp->nearPlaneLength;
    nearplaneY = (tan(fieldOfView)*frustumComp->nearPlaneLength)/aspectRatio;
    nearplaneZ = frustumComp->nearPlaneLength;

    frustumComp->rightTopFar     = gsl::Vector3D(farplaneX, farplaneY, farplaneZ);
    frustumComp->rightBottomFar  = gsl::Vector3D(farplaneX, -farplaneY, farplaneZ);
    frustumComp->leftTopFar      = gsl::Vector3D(-farplaneX, farplaneY, farplaneZ);
    frustumComp->leftBottomFar   = gsl::Vector3D(-farplaneX, -farplaneY, farplaneZ);

    frustumComp->rightTopNear    = gsl::Vector3D(nearplaneX, nearplaneY, nearplaneZ);
    frustumComp->rightBottomNear = gsl::Vector3D(nearplaneX, -nearplaneY, nearplaneZ);
    frustumComp->leftTopNear     = gsl::Vector3D(-nearplaneX, nearplaneY, nearplaneZ);
    frustumComp->leftBottomNear  = gsl::Vector3D(-nearplaneX, -nearplaneY, nearplaneZ);

}

gsl::Vector3D Camera::position() const
{
    return mPosition;
}

gsl::Vector3D Camera::up() const
{
    return mUp;
}

void Camera::init(/*GLint matrixUniform[4]*/)
{
    initializeOpenGLFunctions();

       // Set what shader you want to use to render this object
       //mMaterial->setActiveShader(ShaderType::TEXTURE_SHADER);
       //mMaterial->setActiveTextureSlot(2);
       //mMaterial->setupModelMatrixUniform(mMatrixUniform, matrixUniform);

       glGenVertexArrays( 1, &getMeshComponent()->mVAO );
       glBindVertexArray( getMeshComponent()->mVAO );


       glGenBuffers( 1, &getMeshComponent()->mVBO );
       glBindBuffer( GL_ARRAY_BUFFER, getMeshComponent()->mVBO );

       glBufferData( GL_ARRAY_BUFFER,                     //what buffer type
                     getMeshComponent()->mVertices.size() * sizeof( Vertex ), //how big buffer do we need
                     getMeshComponent()->mVertices.data(),                    //the actual vertices
                     GL_STATIC_DRAW                       //should the buffer be updated on the GPU
                     );

       glBindBuffer(GL_ARRAY_BUFFER, getMeshComponent()->mVBO);

       // 1rst attribute buffer : coordinates

       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(0));
       glEnableVertexAttribArray(0);

       // 2nd attribute buffer : colors
       glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)) );
       glEnableVertexAttribArray(1);

       // 3rd attribute buffer : uvs
       glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
       glEnableVertexAttribArray(2);

       glBindVertexArray(0);
}

void Camera::draw()
{
    glBindVertexArray( getMeshComponent()->mVAO );
    glDrawArrays(GL_LINE, 0, getMeshComponent()->mVertices.size());
    glBindVertexArray(0);
}


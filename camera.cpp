#include "camera.h"
#include "math.h"
#include "matrix3x3.h"
#include "matrix4x4.h"
#include "vector4d.h"
#include "math_constants.h"
#include <QDebug>


Camera::Camera(float fieldOfView, float aspectRatio)
{
    mViewMatrix.setToIdentity();
    mProjectionMatrix.setToIdentity();

    mYawMatrix.setToIdentity();
    mPitchMatrix.setToIdentity();
    frustumComp = new FrustumCollisionComponent();
    frustumComp->mMatrix.setToIdentity();
    frustumComp->farPlaneLength  = 50.0f;
    frustumComp->nearPlaneLength = 10.0f;

    aRatio = 4/3;
    FOV = 90;

    farplaneX  = tan(fieldOfView)*frustumComp->farPlaneLength;
    farplaneY  = (tan(fieldOfView)*frustumComp->farPlaneLength)/aspectRatio;
    farplaneZ  = frustumComp->farPlaneLength;
    nearplaneX = tan(fieldOfView)*frustumComp->nearPlaneLength;
    nearplaneY = (tan(fieldOfView)*frustumComp->nearPlaneLength)/aspectRatio;
    nearplaneZ = frustumComp->nearPlaneLength;

    nearPlaneTopRight    = gsl::Vector3D( nearplaneX,  nearplaneY, -nearplaneZ);
    nearPlaneTopLeft     = gsl::Vector3D(-nearplaneX,  nearplaneY, -nearplaneZ);
    nearPlaneBottomLeft  = gsl::Vector3D(-nearplaneX, -nearplaneY, -nearplaneZ);
    nearPlaneBottomRight = gsl::Vector3D( nearplaneX, -nearplaneY, -nearplaneZ);

    farPlaneTopRight    = gsl::Vector3D( farplaneX,  farplaneY, -farplaneZ);
    farPlaneTopLeft     = gsl::Vector3D(-farplaneX,  farplaneY, -farplaneZ);
    farPlaneBottomLeft  = gsl::Vector3D(-farplaneX, -farplaneY, -farplaneZ);
    farPlaneBottomRight = gsl::Vector3D( farplaneX, -farplaneY, -farplaneZ);



    rightPlaneNormal   = gsl::Vector3D::cross(nearPlaneBottomRight - farPlaneBottomRight
                                             ,nearPlaneBottomRight - nearPlaneTopRight);
    leftPlaneNormal    = gsl::Vector3D::cross(nearPlaneTopLeft - farPlaneTopLeft
                                             ,nearPlaneTopLeft - nearPlaneBottomLeft);
    topPlaneNormal     = gsl::Vector3D::cross(nearPlaneTopRight - farPlaneTopRight
                                             ,nearPlaneTopRight - nearPlaneTopLeft);
    bottomPlaneNormal  = gsl::Vector3D::cross(nearPlaneBottomLeft - farPlaneBottomLeft
                                             ,nearPlaneBottomLeft - nearPlaneBottomRight);
    nearPlaneNormal    = gsl::Vector3D::cross(nearPlaneBottomRight - nearPlaneTopRight
                                             ,nearPlaneBottomRight - nearPlaneBottomLeft);
    farPlaneNormal     = gsl::Vector3D::cross(farPlaneBottomLeft - farPlaneTopLeft
                                             ,farPlaneBottomLeft - farPlaneBottomRight);
    rightPlaneNormal.normalize();
    leftPlaneNormal.normalize();
    topPlaneNormal.normalize();
    bottomPlaneNormal.normalize();
    nearPlaneNormal.normalize();
    farPlaneNormal.normalize();

    qDebug() << " Normal x: " << rightPlaneNormal.x << " Normal y: " << rightPlaneNormal.y << " Normal z: "<< rightPlaneNormal.z;

    //Nearplane right triangle
    getMeshComponent()->mVertices.push_back(Vertex{0.0f, 0.0f,  -0.5f,                       0.0f, 1.0f, 0.0f,    0.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{nearplaneX, -nearplaneY, -nearplaneZ,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{nearplaneX, nearplaneY, -nearplaneZ,     0.0f, 1.0f, 0.0f,    0.0f, 1.0f});
    //Nearplane left triangle
    getMeshComponent()->mVertices.push_back(Vertex{0.0f, 0.0f,  -0.5f,                       0.0f, 1.0f, 0.0f,    0.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{nearplaneX, -nearplaneY, -nearplaneZ,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-nearplaneX, -nearplaneY, -nearplaneZ,   0.0f, 1.0f, 0.0f,    0.0f, 1.0f});
    //Nearplane top triangle
    getMeshComponent()->mVertices.push_back(Vertex{0.0f, 0.0f,  -0.5f,                       0.0f, 1.0f, 0.0f,    0.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{nearplaneX, nearplaneY, -nearplaneZ,     0.0f, 1.0f, 0.0f,    0.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-nearplaneX, nearplaneY, -nearplaneZ,    0.0f, 1.0f, 0.0f,    0.0f, 1.0f});
    //Nearplane bottom trianlge
    getMeshComponent()->mVertices.push_back(Vertex{0.0f, 0.0f,  -0.5f,                       0.0f, 1.0f, 0.0f,    0.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{nearplaneX, -nearplaneY, -nearplaneZ,    0.0f, 1.0f, 0.0f,    0.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-nearplaneX, -nearplaneY, -nearplaneZ,   0.0f, 1.0f, 0.0f,    0.0f, 1.0f});
    //Nearplane end square
    getMeshComponent()->mVertices.push_back(Vertex{nearplaneX, nearplaneY, -nearplaneZ,     0.0f, 1.0f, 0.0f,    0.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{nearplaneX, -nearplaneY, -nearplaneZ,    0.0f, 1.0f, 0.0f,    0.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-nearplaneX, -nearplaneY, -nearplaneZ,   0.0f, 1.0f, 0.0f,    0.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-nearplaneX, -nearplaneY, -nearplaneZ,   0.0f, 1.0f, 0.0f,    0.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-nearplaneX, nearplaneY, -nearplaneZ,    0.0f, 1.0f, 0.0f,    0.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{nearplaneX, nearplaneY, -nearplaneZ,     0.0f, 1.0f, 0.0f,    0.0f, 1.0f});

    //Farplane
    //Farplane right square
    getMeshComponent()->mVertices.push_back(Vertex{nearplaneX, -nearplaneY, -nearplaneZ,    1.0f, 0.0f, 0.0f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{nearplaneX, nearplaneY, -nearplaneZ,     1.0f, 0.0f, 0.0f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{farplaneX, farplaneY, -farplaneZ,        1.0f, 0.0f, 0.0f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{nearplaneX, -nearplaneY, -nearplaneZ,    1.0f, 0.0f, 0.0f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{farplaneX, -farplaneY, -farplaneZ,       1.0f, 0.0f, 0.0f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{farplaneX, farplaneY, -farplaneZ,        1.0f, 0.0f, 0.0f,    1.0f, 0.0f});

    //Farplane left square
    getMeshComponent()->mVertices.push_back(Vertex{-nearplaneX, -nearplaneY, -nearplaneZ,   1.0f, 0.0f, 0.0f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-nearplaneX, nearplaneY, -nearplaneZ,    1.0f, 0.0f, 0.0f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-farplaneX, farplaneY, -farplaneZ,       1.0f, 0.0f, 0.0f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-nearplaneX, -nearplaneY, -nearplaneZ,   1.0f, 0.0f, 0.0f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-farplaneX, -farplaneY, -farplaneZ,      1.0f, 0.0f, 0.0f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-farplaneX, farplaneY, -farplaneZ,       1.0f, 0.0f, 0.0f,    1.0f, 0.0f});

    //Farplane top square
    getMeshComponent()->mVertices.push_back(Vertex{nearplaneX, nearplaneY, -nearplaneZ,     1.0f, 0.0f, 0.0f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-nearplaneX, nearplaneY, -nearplaneZ,    1.0f, 0.0f, 0.0f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-farplaneX, farplaneY, -farplaneZ,       1.0f, 0.0f, 0.0f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{nearplaneX, nearplaneY, -nearplaneZ,     1.0f, 0.0f, 0.0f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{farplaneX, farplaneY, -farplaneZ,        1.0f, 0.0f, 0.0f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-farplaneX, farplaneY, -farplaneZ,       1.0f, 0.0f, 0.0f,    1.0f, 0.0f});

    //Farplane bottom square
    getMeshComponent()->mVertices.push_back(Vertex{nearplaneX, -nearplaneY, -nearplaneZ,    1.0f, 0.0f, 0.0f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-nearplaneX, -nearplaneY, -nearplaneZ,   1.0f, 0.0f, 0.0f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-farplaneX, -farplaneY, -farplaneZ,      1.0f, 0.0f, 0.0f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{nearplaneX, -nearplaneY, -nearplaneZ,    1.0f, 0.0f, 0.0f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{farplaneX, -farplaneY, -farplaneZ,       1.0f, 0.0f, 0.0f,    1.0f, 0.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-farplaneX, -farplaneY, -farplaneZ,      1.0f, 0.0f, 0.0f,    1.0f, 0.0f});

    //Farplane end square
    getMeshComponent()->mVertices.push_back(Vertex{farplaneX, farplaneY, -farplaneZ,        1.0f, 0.0f, 0.0f,    0.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{farplaneX, -farplaneY, -farplaneZ,       1.0f, 0.0f, 0.0f,    0.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-farplaneX, -farplaneY, -farplaneZ,      1.0f, 0.0f, 0.0f,    0.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-farplaneX, -farplaneY, -farplaneZ,      1.0f, 0.0f, 0.0f,    0.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{-farplaneX, farplaneY, -farplaneZ,       1.0f, 0.0f, 0.0f,    0.0f, 1.0f});
    getMeshComponent()->mVertices.push_back(Vertex{farplaneX, farplaneY, -farplaneZ,        1.0f, 0.0f, 0.0f,    0.0f, 1.0f});

    qDebug() << rightPlaneNormal;

    updateForwardVector();
    updateFrustumPos(FOV, aRatio);

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

void Camera::update(float fieldOfView, float aspectRatio)
{
    mYawMatrix.setToIdentity();
    mPitchMatrix.setToIdentity();

    mPitchMatrix.rotateX(mPitch);
    mYawMatrix.rotateY(mYaw);

    mPosition -= mForward * mSpeed;

    mViewMatrix = mPitchMatrix* mYawMatrix;
    mViewMatrix.translate(-mPosition);

    updateFrustumPos(aRatio, FOV);

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
    //Update frustum
    frustumComp->mMatrix.setToIdentity();
    frustumComp->mMatrix.setPosition(mPosition.x, mPosition.y, mPosition.z);
    frustumComp->mMatrix.setRotation(mPitch, mYaw, 0);
    qDebug() << "FrustumRotation: ";
    qDebug() << frustumComp->mMatrix.getRotation();


/*    farplaneX  = farplaneX  + frustumComp->mMatrix.getPosition().x;
    farplaneY  = farplaneY  + frustumComp->mMatrix.getPosition().y;
    farplaneZ  = farplaneZ  + frustumComp->mMatrix.getPosition().z;
    nearplaneX = nearplaneX + frustumComp->mMatrix.getPosition().x;
    nearplaneY = nearplaneY + frustumComp->mMatrix.getPosition().y;
    nearplaneZ = nearplaneZ + frustumComp->mMatrix.getPosition().z;
*/
    //qDebug() << "Farplanes: " << farplaneX << ", " << farplaneY << ", " << farplaneZ;
    //qDebug() << "Nearplanes: " << nearplaneX << ", " << nearplaneY << ", " << nearplaneZ;

    gsl::Matrix3x3 temp = mViewMatrix.toMatrix3();

    gsl::Matrix4x4 temp2 = {temp.matrix[0],temp.matrix[1],temp.matrix[2], 1,
                           temp.matrix[3], temp.matrix[4], temp.matrix[5], 1,
                           temp.matrix[6], temp.matrix[7], temp.matrix[8], 1,
                           1,1,1,1};

    //Setter opp punkter for normalene med offset fra posisjon til frustumet
    nearPlaneTopRight    = (gsl::Vector3D( nearplaneX,  nearplaneY, -nearplaneZ) + mViewMatrix.getPosition());
    nearPlaneTopLeft     = (gsl::Vector3D(-nearplaneX,  nearplaneY, -nearplaneZ) + mViewMatrix.getPosition());
    nearPlaneBottomLeft  = (gsl::Vector3D(-nearplaneX, -nearplaneY, -nearplaneZ) + mViewMatrix.getPosition());
    nearPlaneBottomRight = (gsl::Vector3D( nearplaneX, -nearplaneY, -nearplaneZ) + mViewMatrix.getPosition());

    farPlaneTopRight     = (gsl::Vector3D( farplaneX,  farplaneY, -farplaneZ) + mViewMatrix.getPosition());
    farPlaneTopLeft      = (gsl::Vector3D(-farplaneX,  farplaneY, -farplaneZ) + mViewMatrix.getPosition());
    farPlaneBottomLeft   = (gsl::Vector3D(-farplaneX, -farplaneY, -farplaneZ) + mViewMatrix.getPosition());
    farPlaneBottomRight  = (gsl::Vector3D( farplaneX, -farplaneY, -farplaneZ) + mViewMatrix.getPosition());

    //Lager normalene ut ifra punkter
    rightPlaneNormal   = gsl::Vector3D::cross(nearPlaneBottomRight - farPlaneBottomRight
                                             ,nearPlaneBottomRight - nearPlaneTopRight);
    leftPlaneNormal    = gsl::Vector3D::cross(nearPlaneTopLeft - farPlaneTopLeft
                                             ,nearPlaneTopLeft - nearPlaneBottomLeft);
    topPlaneNormal     = gsl::Vector3D::cross(nearPlaneTopRight - farPlaneTopRight
                                             ,nearPlaneTopRight - nearPlaneTopLeft);
    bottomPlaneNormal  = gsl::Vector3D::cross(nearPlaneBottomLeft - farPlaneBottomLeft
                                             ,nearPlaneBottomLeft - nearPlaneBottomRight);
    nearPlaneNormal    = gsl::Vector3D::cross(nearPlaneBottomRight - nearPlaneTopRight
                                             ,nearPlaneBottomRight - nearPlaneBottomLeft);
    farPlaneNormal     = gsl::Vector3D::cross(farPlaneBottomLeft - farPlaneTopLeft
                                             ,farPlaneBottomLeft - farPlaneBottomRight);
/*
    rightPlaneNormal4D = gsl::Vector4D(rightPlaneNormal.x,rightPlaneNormal.y, rightPlaneNormal.z, 1);
    leftPlaneNormal4D = gsl::Vector4D(leftPlaneNormal.x,leftPlaneNormal.y, leftPlaneNormal.z, 1);
*/

    //Roterer normalene
    rightPlaneNormal = temp*rightPlaneNormal;
    leftPlaneNormal = temp*leftPlaneNormal;
    topPlaneNormal = temp*topPlaneNormal;
    bottomPlaneNormal = temp*bottomPlaneNormal;
    nearPlaneNormal = temp*nearPlaneNormal;
    farPlaneNormal = temp*farPlaneNormal;

/*
    float halfVheight = frustumComp->farPlaneLength * tanf(gsl::deg2rad(fieldOfView)); //calculate the lenght of the opposite
    float halfHwidth = halfVheight * (aspectRatio);

    float horisontalHalfAngle = abs(gsl::rad2degf(atan2f(halfHwidth, frustumComp->farPlaneLength)));

    gsl::Vector3D tempVector;
    //rightplane vector = mRight rotated by FOV around camera up
    tempVector = mRight;
    tempVector.axisAngleRotation(-horisontalHalfAngle, mUp);
    rightPlaneNormal = tempVector.normalized();

    //leftPlane vector = mRight rotated by FOV+180 around camera up
    tempVector = mRight;
    tempVector.axisAngleRotation(horisontalHalfAngle - 180, mUp);
    leftPlaneNormal = tempVector.normalized();

*/
    rightPlaneNormal.normalize();
    leftPlaneNormal.normalize();
    topPlaneNormal.normalize();
    bottomPlaneNormal.normalize();
    nearPlaneNormal.normalize();
    farPlaneNormal.normalize();



    qDebug() << "RightplaneNormal: ";
    qDebug() << rightPlaneNormal;
    qDebug() << "LeftplaneNormal: ";
    qDebug() << leftPlaneNormal;

    /*
    qDebug() << "rightPlaneNormal: " << rightPlaneNormal;
    qDebug() << "leftPlaneNormal: " << leftPlaneNormal;

    qDebug() << "topPlaneNormal: " << topPlaneNormal;
    qDebug() << "bottomPlaneNormal: " << bottomPlaneNormal;

    qDebug() << "nearPlaneNormal: " << nearPlaneNormal;
    qDebug() << "farPlaneNormal: " << farPlaneNormal;
    */
    qDebug() << "Camera position: ";
    qDebug() << mPosition;
    qDebug() << "Frustum position: ";
    qDebug() << frustumComp->mMatrix.getPosition();
    qDebug() << "Rotation: ";
    qDebug() << frustumComp->mMatrix.getRotation();

}

void Camera::lookat(gsl::Vector3D &position, gsl::Vector3D &at, const gsl::Vector3D &up)
{
  mViewMatrix.lookAt(position,at,up);
   mYaw = -90.f, mPitch = -30.f;
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
    glDrawArrays(GL_LINE_STRIP, 0, getMeshComponent()->mVertices.size());
    glBindVertexArray(0);
}

void Camera::move(float x, float y, float z)
{
    //dsgetTransformComponent()->mMatrix.translate(x,y,z);
    //getFrustumComponent()->mMatrix.translate(x,y,z);
}


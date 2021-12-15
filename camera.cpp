#include "camera.h"
#include "mathfunctions.h"
#include "visualobject.h"

Camera::Camera()
{
    m_eye = QVector3D(0,50,5);

    m_lockedPositionOffset = (QVector3D(0, 7, -15));
    m_projectionMatrix.setToIdentity();
    m_viewMatrix.setToIdentity();

    m_pitchMatrix.setToIdentity();
    m_yawMatrix.setToIdentity();
}

void Camera::perspective(float degrees, float aspectRatio, float nearplane, float farplane)
{
    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(degrees, aspectRatio, nearplane, farplane);
}

void Camera::lookAt(const QVector3D &at, const QVector3D &up)
{
    m_viewMatrix.setToIdentity();
    m_viewMatrix.lookAt(m_eye, at, up);
}

void Camera::update(GLint projectionMatrix, GLint viewMatrix)
{
    initializeOpenGLFunctions();

    if(m_editorCamera)
    {
        m_viewMatrix = QMatrix4x4
                (
                 m_right.x(),   m_right.y(),   m_right.z(),   0.f,
                 m_up.x(),      m_up.y(),      m_up.z(),      0.f,
                 m_forward.x(), m_forward.y(), m_forward.z(), 0.f,
                 0.f,           0.f,           0.f,           1.f
                );

        gsm::MathFunctions::rotateX(m_pitch, m_pitchMatrix);

        m_viewMatrix.translate(-m_position);
    }
    else
    {
        m_viewMatrix.setToIdentity();
        QVector3D targetPos = m_target->getPosition();
        m_viewMatrix.lookAt(targetPos + m_lockedPositionOffset, targetPos, QVector3D(0,1,0));
    }

    glUniformMatrix4fv(projectionMatrix, 1, GL_FALSE, m_projectionMatrix.constData());
    glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, m_viewMatrix.constData());
}

void Camera::translate(QVector3D vector)
{
    m_viewMatrix.translate(vector.x(), vector.y(), vector.z());
}

void Camera::moveRight(float deltaLocation)
{
    QVector3D right = m_right;
    right.setY(0.f);
    m_position += right * deltaLocation;
}

void Camera::moveForward(float deltaLocation)
{
    m_position -= m_forward * deltaLocation;
}

void Camera::moveUp(float deltaLocation)
{
    m_position += QVector3D{0.f, 1.f, 0.f} * deltaLocation;
}

void Camera::pitch(float degrees)
{
    m_pitch += degrees;

    if(m_pitch > 89.f)
        m_pitch = 89.f;
    if(m_pitch < -89.f)
        m_pitch = -89.f;

    updateForwardVector();
}

void Camera::yaw(float degrees)
{
    m_yaw += degrees;
    updateForwardVector();
}

//call first
void Camera::updateForwardVector()
{
    QVector3D dir{};
    float yawRad = gsm::MathFunctions::degreesToRadians(m_yaw);
    float pitchRad = gsm::MathFunctions::degreesToRadians(m_pitch);

    dir.setX(cos(yawRad) * cos(pitchRad)); //X == X :)
    dir.setY(sin(pitchRad)); //Up in world space
    dir.setZ(sin(yawRad) * cos(pitchRad)); //Actually Y in world space
    m_forward = dir;
    m_forward.normalize();

    updateRightVector();
    updateUpVector();
}

//Call before updateUpVector()
void Camera::updateRightVector()
{
    m_up = {0.f, 1.f, 0.f};
    m_right = QVector3D::crossProduct(m_up, m_forward);
    m_right.normalize();
}

//Call after updateRightVector()
void Camera::updateUpVector()
{
    m_up = QVector3D::crossProduct(m_forward, m_right);
}

void Camera::setPosition(const QVector3D &position)
{
    m_position = position;
}

QVector3D Camera::position() const
{
    return m_position;
}

QVector3D Camera::up() const
{
    return m_up;
}

bool Camera::editorCamera() const
{
    return m_editorCamera;
}

void Camera::setEditorCamera(bool editorCamera)
{
    m_editorCamera = editorCamera;
}

void Camera::setName(const std::string &name)
{
    m_name = name;
}

void Camera::setTarget(VisualObject *target)
{
    m_target = target;
}

QMatrix4x4 Camera::viewMatrix() const
{
    return m_viewMatrix;
}

QMatrix4x4 Camera::projectionMatrix() const
{
    return m_projectionMatrix;
}

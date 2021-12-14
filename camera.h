#ifndef CAMERA_H
#define CAMERA_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>

class Camera : public QOpenGLFunctions_4_1_Core
{
public:
    Camera();

    void perspective(float degrees, float aspect, float nearplane, float farplane);
    void lookAt(const QVector3D& at, const QVector3D& up);
    void translate(QVector3D vector);

    void moveRight(float deltaLocation);
    void moveForward(float deltaLocation);
    void moveUp(float deltaLocation);
    void pitch(float degrees);
    void yaw(float degrees);

    //Updates
    void update(GLint projectionMatrix, GLint viewMatrix);
    void updateRightVector();
    void updateForwardVector();
    void updateUpVector();

    void setPosition(const QVector3D &position);
    void setTarget(class VisualObject *target);
    void setEditorCamera(bool editorCamera);
    void setName(const std::string& name);

    QMatrix4x4 viewMatrix() const;
    QMatrix4x4 projectionMatrix() const;
    QVector3D position() const;
    QVector3D up() const;
    bool editorCamera() const;


private:
    QVector3D m_eye{};
    std::string m_name{};

    //Direction, position and orientation
    QVector3D m_position{};
    QVector3D m_right{1.f,0.f,0.f};
    QVector3D m_forward{0.f,0.f,1.f};
    QVector3D m_up{0.f, 1.f, 0.f};
    float m_pitch{};
    float m_yaw{};

    //Locked camera
    float m_cameraOffset{};
    bool m_editorCamera{true};
    QVector3D m_lockedPositionOffset{}; //Used to point at where the camera should be in 3rd person mode

    //Matrices
    QMatrix4x4 m_projectionMatrix{};
    QMatrix4x4 m_viewMatrix{};
    QMatrix4x4 m_yawMatrix{};
    QMatrix4x4 m_pitchMatrix{};

    class VisualObject* m_target; //For use in 3rd person mode

};

#endif // CAMERA_H

#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H
#include <QKeyEvent>
#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include "camerainputcomponent.h"
#include "playerinputcomponent.h"

class InputSystem : public QWindow, protected QOpenGLFunctions_4_1_Core
{
public:
    InputSystem();

private:
    Input* mInput;
//    float mCameraSpeed{0.05f};
//    float mCameraRotateSpeed{0.1f};
//    int mMouseXlast{0};
//    int mMouseYlast{0};

//    void setCameraSpeed(float value);

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
//  virtual   void mouseMoveEvent(QMouseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
//    virtual void wheelEvent(QWheelEvent *event) override;
};
#endif // INPUTSYSTEM_H

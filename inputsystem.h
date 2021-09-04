#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H
#include <QKeyEvent>
#include "camera.h"
#include "input.h"
#include "camerainputcomponent.h"
#include "mainwindow.h"


class InputSystem
{
public:
    InputSystem();
    Camera mCurrentCamera;
    Input mInput;
    MainWindow *mMainWindow;

private:
    float mCameraSpeed{0.05f};
    float mCameraRotateSpeed{0.1f};
    int mMouseXlast{0};
    int mMouseYlast{0};
    void setCameraSpeed(float value);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
};

#endif // INPUTSYSTEM_H

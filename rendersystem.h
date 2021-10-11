#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>

#include "input.h"
#include "constants.h"

//Just to debug mousePickingRay
#include "meshhandler.h"

class QOpenGLContext;
class ShaderHandler;
class MainWindow;
class GameObject;
class Camera;
class TextureHandler;

// This inherits from QWindow to get access to the Qt functionality and
// OpenGL surface.
// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
// This is the same as using glad and glw from general OpenGL tutorials
class RenderSystem : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderSystem(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderSystem() override;

    QOpenGLContext *context() { return mContext; }

    void exposeEvent(QExposeEvent *) override;

    void toggleWireframe(bool buttonState);
    void toggleBacksideCulling(bool state);

    //Editor functions
    void setPickedObject(int pickedID);
    void cancelPickedObject();
    //Just to test mousePickingRay
    int mIndexToPickedObject{-1};

    std::vector<GameObject*> mGameObjects;  //should probably not belong to renderer

    bool isPlaying{false};  //is the game playing?

    Camera *mEditorCamera{nullptr};
    Camera *mGameCamera{nullptr};

    void render();

    bool mUseFrustumCulling{false};
    bool mGameCamAsFrustumCulling{false};

private:
    void init();

    void checkForGLerrors();

    void calculateFramerate();

    void startOpenGLDebugger();

    bool frustumCulling(int gameObjectIndex);

    void mousePickingRay(QMouseEvent *event);

    class TextureHandler *mTextures[gsl::NumberOfTextures]{nullptr}; //We can hold some textures

    class ShaderHandler *mShaderPrograms[gsl::NumberOfShaders]{nullptr};    //holds pointer the GLSL shader programs

    //these can maybe be removed, because they are held in the camera
    float mAspectratio{1.f};
    float mFOVangle{45};

    int mMouseXlast{0};
    int mMouseYlast{0};

    //for statistics in status bar
    int mVerticesDrawn{0};
    int mObjectsDrawn{0};

    QOpenGLContext *mContext{nullptr};
    bool mInitialized;

    QElapsedTimer mTimeStart;       //time variable that reads the calculated FPS

    MainWindow *mMainWindow{nullptr};        //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};
    class Logger *mLogger{nullptr};

protected:
    //The QWindow that we inherit from has these functions to capture mouse and keyboard.
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
};

#endif // RENDERSYSTEM_H

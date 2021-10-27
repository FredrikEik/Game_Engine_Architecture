#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>

#include "input.h"
#include "constants.h"
#include "inputsystem.h"
#include "component.h"
#include "shapefactory.h"
#include "soundmanager.h"
#include "soundsource.h"

class QOpenGLContext;
class Shader;
class MainWindow;
class VisualObject;
class Camera;
class Texture;

// This inherits from QWindow to get access to the Qt functionality and
// OpenGL surface.
// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
// This is the same as using glad and glw from general OpenGL tutorials
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;

    QOpenGLContext *context() { return mContext; }

    void exposeEvent(QExposeEvent *) override;

    void toggleWireframe(bool buttonState);
    void playMode(bool p);
    void toggleShapes(int shapeID);
    void setCameraSpeed(float value);
    std::vector<NameComponent*> mNameComps;
    std::vector<TransformComponent*> mTransComps;

private slots:
    void render();

private:
    SoundSource* mLaserSound{};
    InputComponent *mInputComponent;
    InputSystem *mInputSystem;
    CollisionSystem* mCollisionSystem;
    FrustumSystem* mFrustumSystem;
    ShapeFactory mShapeFactory;
    static const int nrOfShapes = 5;
    VisualObject* myShapes[nrOfShapes];
    Input mInput;
    Player* mPlayer;
    bool shapeExist[5];
    bool playM = false;


    MeshComponent mDebugMousePickRay;
    void mousePickingRay(QMouseEvent *event);
    bool mDrawMousePickRay{false};
    gsl::Vector3D mMousePickRay;
    gsl::Vector3D ray_wor;

    void init();
    void initObject();
    void drawObject();


    void checkForGLerrors();

    void calculateFramerate();

    void startOpenGLDebugger();

    void setupPlainShader(int shaderIndex);
    GLint mMatrixUniform{-1};
    GLint vMatrixUniform{-1};
    GLint pMatrixUniform{-1};

    void setupTextureShader(int shaderIndex);
    GLint mMatrixUniform1{-1};
    GLint vMatrixUniform1{-1};
    GLint pMatrixUniform1{-1};
    GLint mTextureUniform{-1};

    class Texture *mTextures[gsl::NumberOfTextures]{nullptr}; //We can hold some textures

    class Shader *mShaderPrograms[gsl::NumberOfShaders]{nullptr};    //holds pointer the GLSL shader programs

    Camera *mCurrentCamera{nullptr};
    Camera mEditorCamera;
    Camera mPlayCamera;

    std::vector<VisualObject*> mVisualObjects;

    QOpenGLContext *mContext{nullptr};
    bool mInitialized;

    QTimer *mRenderTimer{nullptr};           //timer that drives the gameloop
    QElapsedTimer mTimeStart;       //time variable that reads the calculated FPS

    MainWindow *mMainWindow{nullptr};        //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

};

#endif // RENDERWINDOW_H

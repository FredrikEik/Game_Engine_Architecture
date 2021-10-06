#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>
#include "gameobject.h"
#include "factory.h"
#include "input.h"
#include "constants.h"
#include "soundmanager.h"
#include "quadtree.h"

class QOpenGLContext;
class Shader;
class MainWindow;
class GameObject;
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

    void setPickedObject(int pickedID);
    void cancelPickedObject();

    void createObjectbutton(std::string objectName);
    void playPausebutton();
    bool bPause = true;

    Factory *getFactory(){return factory;};

    SoundSource* mClick{};
    SoundSource* mVideoGameLand{};


private slots:
    void render();

private:
    void init();

    void checkForGLerrors();

    void calculateFramerate();

    void startOpenGLDebugger();

    bool isColliding(BoxCollisionComponent &Box1, BoxCollisionComponent &Box2);
	bool isColliding(BoxCollisionComponent &Box, SphereCollisionComponent &Sphere);
    bool isInFrustum(FrustumCollisionComponent &);

    void setCameraSpeed(float value);

    void handleInput();


    GLint mMatrixUniform[4];
    GLint vMatrixUniform[4];
    GLint pMatrixUniform[4];

    void setupPlainShader(int shaderIndex);
    void setupTextureShader(int shaderIndex);

    GLint mTextureUniform{-1};

    class Texture *mTextures[gsl::NumberOfTextures]{nullptr}; //We can hold some textures

    class Shader *mShaderPrograms[gsl::NumberOfShaders]{nullptr};    //holds pointer the GLSL shader programs

    Camera* mCurrentCamera{nullptr};
    Camera* mTestFrustumCamera{nullptr};

    float mAspectratio{1.f};

    class Factory* factory = new Factory;
    class gsml::Quadtree<uint32_t, GameObject*> mQuadtree;


    int mIndexToPickedObject{-1};

    //std::vector<VisualObject*> mVisualObjects;


    Input mInput;
    float mCameraSpeed{0.05f};
    float mCameraRotateSpeed{0.1f};
    int mMouseXlast{0};
    int mMouseYlast{0};

    QOpenGLContext *mContext{nullptr};
    bool mInitialized;

    QTimer *mRenderTimer{nullptr};           //timer that drives the gameloop
    QElapsedTimer mTimeStart;       //time variable that reads the calculated FPS

    MainWindow *mMainWindow{nullptr};        //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};

protected:
    //The QWindow that we inherit from has these functions to capture mouse and keyboard.
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePicking(QMouseEvent *event);
};

#endif // RENDERWINDOW_H

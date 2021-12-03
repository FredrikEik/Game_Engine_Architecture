#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>

#include "input.h"
#include "constants.h"
#include "objectfactory.h"
#include "gameobject.h"
#include "soundmanager.h"
#include "mousepicker.h"
#include "spawner.h"
#include "player.h"
#include "skybox.h"
#include "script.h"
#include "terrain.h"
#include "rollingball.h"


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
    void toggleBacksideCulling(bool state);

    std::vector<GameObject*> mGameObjects;

    bool mIsPlaying{false};  //is the game playing?

    Camera *mEditorCamera{nullptr};
    Camera *mGameCamera{nullptr};

    player* mPlayer { nullptr };

    RollingBall* mRollingBall { nullptr };

    bool bRoll { false };
    void startRoll() { bRoll = true; }

    void playSound();
    void pauseSound();
    void stopSound();
//    void valueX(double arg1);
    void setSelectionIndex(int index);
    void deleteSelection();

    gsl::Matrix4x4 getTransform(int index);
    gsl::Vector3D getRotation(int index);
    gsl::Vector3D getScale(int index);

    void setPositionX(double value);
    void setPositionY(double value);
    void setPositionZ(double value);

    void setRotationX(double value);
    void setRotationY(double value);
    void setRotationZ(double value);

    void setScaleX(double value);
    void setScaleY(double value);
    void setScaleZ(double value);
    void setScale(double x, double y, double z, int index);

    GameObject* mSurface;
    GameObject* mContour;



    void updateMatrix();

    void toggleGameMode();

    void mousePickingRay(QMouseEvent *event);

    bool mUseFrustumCulling{true};
    bool mGameCamAsFrustumCulling{false};

    int mTextureIndex = 0;

    Script* mScript { nullptr };

private slots:
    void render();

private:
    void init();

    void checkForGLerrors();

    void calculateFramerate();

    void startOpenGLDebugger();

    bool frustumCulling(int gameObjectIndex);

    bool objectsColliding(CollisionComponent Box1, CollisionComponent Box2, TransformComponent Box1trans, TransformComponent Box2trans);

    void setCameraSpeed(float value);

    void handleInput();

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
    float mAspectratio{1.f};

    ObjectFactory* ObjFactory = new ObjectFactory;
    SoundSource* mStereoSound;
    SoundSource* mPop;
    SoundSource* mJump;
    class Spawner* MapSpawner;
    class SkyBox* skyBox { nullptr };
    class Terrain* terrain { nullptr };

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

    MousePicker *mMousePicker{nullptr};
    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};

    bool bPlayGame = false;

    //for statistics in status bar
    int mVerticesDrawn{0};
    int mObjectsDrawn{0};


protected:
    //The QWindow that we inherit from has these functions to capture mouse and keyboard.
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

public:
    void ObjectButton(std::string object);
};

#endif // RENDERWINDOW_H

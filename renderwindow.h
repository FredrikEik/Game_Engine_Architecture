#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>

#include "input.h"
#include "constants.h"
#include "Components.h"

class QVector3D;
class QOpenGLContext;
class Shader;
class MainWindow;
class GameObject;
class Camera;
class Texture;
class CoreEngine;

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

    static RenderWindow& getInstance();

    void exposeEvent(QExposeEvent *) override;

    void toggleWireframe(bool buttonState);

    void toggleLOD();
    void toggleFrustum();

    Input getInput();
    void setPlayerMovement(float x, float y, float z);

    void addToGameObjects(GameObject *obj);
    std::vector<GameObject*> getGameObjects();
    std::vector<GameObject*>* getGameObjectsPtr();

    double getVertexCount();

    int getSelectedObject();

    void setToCurrentCamera(Camera* cam);
    Camera* getCurrentCamera();

    void render();

    bool bPlayerColliding{false};

    int getIndexToPickedObject();

private:
    void init();

    void checkForGLerrors();

    void checkForCollisions(GameObject* player);

    void calculateFramerate();

    void startOpenGLDebugger();

    void setCameraSpeed(float value);

    void setupPlainShader(int shaderIndex);
    GLint mMatrixUniform{-1};
    GLint vMatrixUniform{-1};
    GLint pMatrixUniform{-1};

    void setupTextureShader(int shaderIndex);
    GLint mMatrixUniform1{-1};
    GLint vMatrixUniform1{-1};
    GLint pMatrixUniform1{-1};
    GLint mTextureUniform{-1};

    void setupPhongShader(int shaderIndex);
    GLint mMatrixUniform2{-1};
    GLint vMatrixUniform2{-1};
    GLint pMatrixUniform2{-1};

    GLint mLightColorUniform{-1};
    GLint mObjectColorUniform{-1};
    GLint mAmbientLightStrengthUniform{-1};
    GLint mLightPositionUniform{-1};
    GLint mCameraPositionUniform{-1};
    GLint mSpecularStrengthUniform{-1};
    GLint mSpecularExponentUniform{-1};
    GLint mLightPowerUniform{-1};
    GLint mPhongTextureUniform{-1};

    void setupSkyboxShader(int shaderIndex);
    GLint mMatrixUniform3{-1};
    GLint vMatrixUniform3{-1};
    GLint pMatrixUniform3{-1};
    GLint mSkyboxUniform{-1};


    class Texture *mTextures[gsl::NumberOfTextures]{nullptr}; //We can hold some textures

    class Shader *mShaderPrograms[gsl::NumberOfShaders]{nullptr};    //holds pointer the GLSL shader programs

    Camera *mCurrentCamera{nullptr};
    float mAspectratio{1.f};

    std::vector<GameObject*> mGameObjects;

    bool bLODToggleEnabled{true};
    bool bFrustumEnabled{true};

    Input mInput;
    float mCameraSpeed{0.05f};
    float mCameraRotateSpeed{0.1f};
    int mMouseXlast{0};
    int mMouseYlast{0};

    float moveX{0}, moveY{0}, moveZ{0};

    int indexToPickedObject{-1};

    QOpenGLContext *mContext{nullptr};
    bool mInitialized;

    QTimer *mRenderTimer{nullptr};           //timer that drives the gameloop
    QElapsedTimer mTimeStart;       //time variable that reads the calculated FPS

    MainWindow *mMainWindow{nullptr};        //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};
    class ResourceManager *mResourceManager{nullptr};
    class CoreEngine *mCoreEngine{nullptr};

    Frustum *frustum{nullptr};

    void mousePicking(QMouseEvent *event);

protected:
    //The QWindow that we inherit from has these functions to capture mouse and keyboard.
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
};

#endif // RENDERWINDOW_H

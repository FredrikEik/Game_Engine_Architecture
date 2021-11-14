#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>

#include "input.h"
#include "constants.h"
#include "meshhandler.h"
#include "collisionhandler.h"

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

    std::vector<GameObject*> mGameObjects;  //should probably not belong to renderer

    std::vector<GameObject*> mParticles;

    std::vector<GameObject*> mProjectiles;

    bool isPlaying{false};  //is the game playing?
    Camera *mCurrentCamera{nullptr};

    void render();

    void rotateObj(double val);

   void setPickedObject(int pickedID);

   void updatePickedObject();







private:

   int mIndexToPickedObject{-1};

    void init();

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

    void updateDt();
    float dt = 0;
    float curTime = 0;
    float lastTime = 0;

    class TextureHandler *mTextures[gsl::NumberOfTextures]{nullptr}; //We can hold some textures

    class ShaderHandler *mShaderPrograms[gsl::NumberOfShaders]{nullptr};    //holds pointer the GLSL shader programs

    float mAspectratio{1.f};

    int mMouseXlast{0};
    int mMouseYlast{0};

    int mVerticesDrawn{0};
    int mObjectsDrawn{0};
    int mParticlesDrawn{0};

    float translateX{1};
    float translateY{1};
    float translateZ{1};
    gsl::Vector3D tempPos;

    float mFOVangle{45};

    QOpenGLContext *mContext{nullptr};
    bool mInitialized;

    QElapsedTimer mTimeStart;       //time variable that reads the calculated FPS
    QElapsedTimer timer;

    MainWindow *mMainWindow{nullptr};        //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};



   // MeshData linebox;
    MeshData linebox2;

    MeshData circle1;
    MeshData circle2;



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

#endif // RENDERSYSTEM_H

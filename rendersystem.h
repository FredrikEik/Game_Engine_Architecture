#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>
#include "timer.h"

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

    std::vector<GameObject*> mGameObjects;

    std::vector<GameObject*> mParticles;

    std::vector<GameObject*> mProjectiles;

    bool isPlaying{false};  //is the game playing?
    Camera *mCurrentCamera{nullptr};

    bool toogleFrustumDrawing = true;
    ///render loop
    void render();

    void rotateObj(double val);

    void timerSetup(bool toggle);
    ///toggler frustum
    void toggleFrustum(bool toggle);


private:


    int mIndexToPickedObject{-1};

    int viewMatrix{-1};
    int projectionMatrix{-1};
    int modelMatrix{-1};

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

    void setupSkyboxShader(int shaderIndex);
    GLint mMatrixUniformSS{-1};
    GLint vMatrixUniformSS{-1};
    GLint pMatrixUniformSS{-1};
    GLint mTextureUniformSS{-1};

    void setupLightShader(int shaderIndex);
    GLint mMatrixUniform2{-1};
    GLint vMatrixUniform2{-1};
    GLint pMatrixUniform2{-1};
    GLint mTextureUniform2{-1};
    GLint mTextureUniform3{-1};


    GLint mCameraPosition{-1};

    GLint mLightPosition{-1};

    GLint mLightDirection{-1};

    GLint mLightColor{-1};
    GLint mObjectColor{-1};

    GLint mAmbientColor{-1};
    GLint mAmbientStrengt{-1};
    GLint mLightStrengt{-1};
    GLint mSpecularStrength{-1};
    GLint mSpecularExponent{-1};

    bool DrawInEditorOnly = false;


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

    int timeTest{0};

    float translateX{1};
    float translateY{1};
    float translateZ{1};
    gsl::Vector3D tempPos;

    float mFOVangle{60};

    QOpenGLContext *mContext{nullptr};
    bool mInitialized;

    QElapsedTimer mTimeStart;       //time variable that reads the calculated FPS

    MainWindow *mMainWindow{nullptr};        //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};

    Timer timer;
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
    ///mousepicking
    void mousePicking(QMouseEvent *event);
};

#endif // RENDERSYSTEM_H

#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>
#include "obj.h"
#include "vector4d.h"
#include "matrix4x4.h"
#include "collisionsystem.h"
#include <math.h>       /* sqrt */
#include "level.h"
#include "player.h"
#include "enemy.h"
#include "particles.h"

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
    void setCameraSpeed(float value);
    void playMode(bool p);
    bool mousePickCollide = false;
    int MousePickindex = 0;

    Level* mLvl;



private slots:
    void render();

private:
    MovementComponent *mMoveComp{nullptr};
    MovementSystem *mMoveSystem{nullptr};
    Player *mPlayer{nullptr};
    Input mInput;
    CollisionSystem* mCollisionSystem{nullptr};
    FrustumSystem* mFrustumSystem{nullptr};
    ShapeFactory mShapeFactory;
    Enemy *mEnemy{nullptr};
    Light * mLight{nullptr};
    Particles *mParticle{nullptr};

    static const int nrOfShapes = 5;
    bool playM = false;


    void mousePickingRay(QMouseEvent *event);
     gsl::Vector3D ray_wor;

    int Lod(int i);
    void drawObjects();

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
    GLint mMatrixUniform3{-1};
    GLint vMatrixUniform3{-1};
    GLint pMatrixUniform3{-1};
    GLint mTextureUniform3{-1};

    void setupPhongShader(int shaderIndex);
    GLint mMatrixUniform2{-1};
    GLint vMatrixUniform2{-1};
    GLint pMatrixUniform2{-1};
    GLint mTextureUniformPhong{-1};
   // GLint mUsingTextureUniform{-1};

    //light shader variables
    GLint mLightColorUniform{-1};
    GLint mObjectColorUniform{-1};
    GLint mAmbientLightStrengthUniform{-1};
    GLint mLightPositionUniform{-1};
    GLint mCameraPositionUniform{-1};
    GLint mSpecularStrengthUniform{-1};
    GLint mSpecularExponentUniform{-1};
    GLint mLightPowerUniform{-1};


    class Texture *mTextures[gsl::NumberOfTextures]{nullptr}; //We can hold some textures

    class Shader *mShaderPrograms[gsl::NumberOfShaders]{nullptr};    //holds pointer the GLSL shader programs

    Camera *mCurrentCamera{nullptr};
    Camera mEditorCamera;
    Camera mPlayCamera;
    float mAspectratio{1.f};
    int frameCount{0};


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

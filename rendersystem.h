#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>
#include <QJSEngine>
#include "gameobject.h"
#include "factory.h"
#include "level.h"
#include "input.h"
#include "constants.h"
#include "soundmanager.h"
#include "quadtree.h"
#include "light.h"
#include "player.h"
#include "shaderhandler.h"

class QOpenGLContext;
class Shader;
class MainWindow;
class GameObject;
class Camera;
class Texture;
class ShaderHandler;
class System;

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

    void initTextures();

    void toggleWireframe(bool buttonState);
    bool toggleFrustumCulling {false};

    float getAspectRatio() {return mAspectratio;}

    void setPickedObject(int pickedID);
    void cancelPickedObject();

    void createObjectbutton(std::string objectName);
    void playPausebutton(const QSurfaceFormat &format);

    /**Textures**/
    class Texture *mTextures[gsl::NumberOfTextures]{nullptr}; //We can hold some textures

    /**ShaderHandler**/
    ShaderHandler* mShaderHandler;

    System* systemRef; //points back to system
    MainWindow *mMainWindow{nullptr};        //points back to MainWindow to be able to put info in StatusBar

    void moveHelpObjectToSelected();

    void render();

private:
    void init();

    void checkForGLerrors();

    void calculateFramerate();

    void startOpenGLDebugger();

    //void generateShadowDepthMap();

    float mAspectratio{1.f};
    int objectsDrawn{0};

    class gsml::Quadtree<uint32_t, GameObject*> mQuadtree;

    int mIndexToPickedObject{-1};

    QOpenGLContext *mContext{nullptr};
    bool mInitialized;

    int mMouseXlast{0};
    int mMouseYlast{0};

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

#endif // RENDERSYSTEM_H

#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

class QOpenGLContext;
class ShaderHandler;
class MainWindow;
class GameObject;
class Camera;
class TextureHandler;
class GameObjectManager;

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

    bool mIsPlaying{false};

    Camera *mEditorCamera{nullptr};
    Camera *mGameCamera{nullptr};

    GameObjectManager *mGameObjectManager{nullptr};

    void render();

    bool mUseFrustumCulling{true};
    bool mGameCamAsFrustumCulling{false};

    bool mDrawAxis{true};
    bool mDrawGrid{true};

    std::vector<GameObject*> getAllGameObjects();
    GameObject getGameObjectOfName(std::string name);

private:
    void init();

    void checkForGLerrors();

    void calculateFramerate();

    void startOpenGLDebugger();

    bool frustumCulling(int gobIndex);

    void mousePickingRay(QMouseEvent *event);

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

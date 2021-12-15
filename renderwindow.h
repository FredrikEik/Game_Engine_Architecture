#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

class ECScoordinator;
class QOpenGLContext;
class Shader;
class Texture;
class MainWindow;
class VisualObject;
class InteractiveObject;
class Camera;

// This inherits from QWindow to get access to the Qt functionality and
// OpenGL surface.
// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
// This is the same as using "glad" and "glw" from general OpenGL tutorials
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT

    friend class ECScoordinator;

public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;

    QOpenGLContext *context() { return mContext; }

    void exposeEvent(QExposeEvent *) override;  //gets called when app is shown and resized

private slots:
    void render();          //the actual render - function

private:
    void init();            //initialize things we need before rendering

    VisualObject* createObject(VisualObject* object, std::string objectName, std::string shaderToUse, QVector3D position); //Used to create a new object for the scene.
    void createAllObjects(/*bool bReset*/);

    //BOSS MAN - This is supposed to oversee most things!
    std::unique_ptr<ECScoordinator> ECScord{nullptr};

    //Data containers
    std::unordered_map<std::string, VisualObject*> m_objectMap; //Primary object container
    VisualObject* m_light{nullptr}; //Scene lightning
    VisualObject* m_world{nullptr}; //The scene map
    VisualObject* m_skybox{nullptr}; //The skybox
    class Player* m_player{nullptr};  //The player
    class RollingBall* m_ball{nullptr}; //The ball

    //Camera
    Camera* m_camera{nullptr};
    Input m_input;
    void setCameraSpeed(float value);
    void setMouseSensitivity(float value);
    float m_cameraSpeed{0.05f};
    float m_cameraRotateSpeed{0.1f};
    float m_cameraAspectRatio{1.f};
    float m_mouseSensitiviy{1.f};
    int m_mouseXlast{0};
    int m_mouseYlast{0};

    int sphere{-1};

    //Debug
    bool bWiremode{false};
    bool bKeyReleased{true};

    QOpenGLContext *mContext{nullptr};  //Our OpenGL context
    bool mInitialized{false};

    //Textures
    std::unordered_map<std::string, Texture*> m_textureMap;

    //Shaders
    std::unordered_map<std::string, Shader*> m_shaderProgramMap;
    Shader* m_shaderDebug{nullptr};

    GLuint mVAO;                        //OpenGL reference to our VAO
    GLuint mVBO;                        //OpenGL reference to our VBO

    QTimer *mRenderTimer{nullptr};           //timer that drives the gameloop
    QElapsedTimer mTimeStart;               //time variable that reads the calculated FPS

    MainWindow *mMainWindow{nullptr};        //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};  //helper class to get some clean debug info from OpenGL

    void checkForGLerrors();            //helper function that uses QOpenGLDebugLogger or plain glGetError()

    void calculateFramerate();          //as name says

    void startOpenGLDebugger();         //starts QOpenGLDebugLogger if possible

    void handleInput();   //Handles the input from mouse/keyboard

protected:
    //The QWindow that we inherit from have these functions to capture
    // - mouse and keyboard.
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
};

#endif // RENDERWINDOW_H

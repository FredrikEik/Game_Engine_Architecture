#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>

#include "input.h"
#include "constants.h"
#include "vector3d.h"
#include "components.h"

class QOpenGLContext;
class ShaderHandler;
class MainWindow;
class GameObject;
class CoreEngine;
class Camera;
class TextureHandler;
class Entity;

// This inherits from QWindow to get access to the Qt functionality and
// OpenGL surface.
// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
// This is the same as using glad and glw from general OpenGL tutorials
class RenderSystem : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    //! init function for open gl
    RenderSystem(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderSystem() override;

    QOpenGLContext *context() { return mContext; }

    void exposeEvent(QExposeEvent *) override;
    //! turns wireframe on or off
    void toggleWireframe(bool buttonState);
    //! @var game object vector array holding the objects in the game
    std::vector<GameObject*> mGameObjects;  //should probably not belong to renderer
    //! @param isPlaying tells if the game is on or off
    bool isPlaying{false};  //is the game playing?
    //! mCurrentCamera pointer to camera that is beeing used
    Camera *mCurrentCamera{nullptr};
    //! @fn mousePicking - picking an object that the mouse is hovering over from screen
    void mousePicking();
    //! @fn render - render loop
    void render();
    //! @fn setPickedObject - setting the object you click on
    //! @param pickedID - id of the object
    void setPickedObject(int pickedID);
    //! @fn cancelPickedObject - deleting the object you picked from the UI
    void cancelPickedObject();
    //! mPickedObject pointer to the pickedobject
    GameObject *mPickedObject;
    //! mMaterialIDs pointer to the material's id
    ecs::MaterialID *mMaterialIDs;
    //! @fn shaderAndMaterialSetup - setting the shader and material for the objects
    void shaderAndMaterialSetup();
    //! @fn rendermousepicker - rendering with colors so the mousepicker works
    //! @param colorUniform - color for rendering
    //! @param matrixUniform - position for rendering
    void rendermousepicker(GLint matrixUniform, GLint colorUniform);

private:
    //! @fn init used to initialise code for open gl and objects in scene
    void init();
    //! @fn error checker
    void checkForGLerrors();
    //! @fn framerate checker
    void calculateFramerate();
    //! @fn debugger
    void startOpenGLDebugger();
    //! @param mTextures - contains pointer to the texture
    class TextureHandler *mTextures[gsl::NumberOfTextures]{nullptr}; //We can hold some textures
    //! @param mShaderPrograms - pointer for all the shaders in shadersystem
    class ShaderHandler *mShaderPrograms[gsl::NumberOfShaders]{nullptr};    //holds pointer the GLSL shader programs
    //! @var aspectratio - aspect ratio of screen
    float mAspectratio{1.f};
    //! @var mRetinaScale - used to display scaling
    qreal mRetinaScale{1};  //used on "double pixle/retina" displays
    //! mPickedObjectMarker - pointer for the object marked by the mousepicker
    GameObject *mPickedObjectMarker{nullptr};
    //! @var mMouseXlast - last x pos of mouse
    int mMouseXlast{0};
    //! @var mMouseYlast - last y pos of mouse
    int mMouseYlast{0};
    //! @var
    int mVerticesDrawn{0};
    int mObjectsDrawn{0};

    float mFOVangle{45};
    //! mCoreEngine - pointer for the core engine
    CoreEngine *mCoreEngine{nullptr};

    QOpenGLContext *mContext{nullptr};
    bool mInitialized;

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
};

#endif // RENDERSYSTEM_H

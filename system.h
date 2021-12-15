#ifndef SYSTEM_H
#define SYSTEM_H

#include <QElapsedTimer>
#include <QJSEngine>
#include <QObject>
#include "components.h"
#include "level.h"

class RenderSystem;
class QTimer;
class QElapsedTimer;
class GameObject;
class Camera;
class Level;
class Player;
class Skybox;
class TriangleSurface;
class MainWindow;

///
/// \brief The System class. Is responsible for the main game loop and events.
///
class System : public QObject
{
    Q_OBJECT
public:
    System(MainWindow *mw, RenderSystem *rs);
    void init();

    ///Starts the game loop timer.
    void startGameLoop();

    /**Level**/
    void saveLevel();
    void loadLevel();
    void clearLevel();
    void resetLevel();

    /**Toggle between play and editor mode**/
    void toggleEditorMode();

    /**Collision**/
    ///Overloaded function for checking AABB vs AABB collision
    bool isColliding(BoxCollisionComponent &Box1, BoxCollisionComponent &Box2);
    ///Overloaded function for checking AABB vs Sphere collision
    bool isColliding(BoxCollisionComponent &Box, SphereCollisionComponent &Sphere);
    ///Overloaded function for checking Sphere vs Frustum collision
    bool isCollidingWithFrustum(SphereCollisionComponent &Sphere);

    /**Input**/
    void handleInput();

    /**Camera**/
    void setCameraSpeed(float value);

    /**Setters**/
    void setRenderSystem(RenderSystem* rs) {renderSystem = rs;}

    /**Getters**/
    InputComponent* getInput() {return input;}
    float getCameraSpeed() {return cameraSpeed;}
    float getCameraRotateSpeed() {return cameraRotateSpeed;}
    bool getEditorMode() {return editorMode;}

    QElapsedTimer timeStart;

    /**Sound**/
    SoundSource* clickSound{};
    SoundSource* marioSound{};
    SoundSource* videoGameLandSound{};
    SoundSource* videoGameLandSound2{};

    /**Failcheck**/
    bool isInitialized {false};

private slots:
    void gameLoop();

private:
    void createMeshes();
    void createObjects();
    void createSounds();

    /**Systems**/
    QOpenGLContext *renderContext{nullptr};
    RenderSystem* renderSystem = nullptr;
    MainWindow* mainWindow = nullptr;
    QTimer *gameLoopTimer = nullptr;
    Level level;

    ///Javascript Engine (Not used)
    QJSEngine scriptEngine;

    /**Input**/
    InputComponent* input = nullptr;

    /**Settings**/
    bool bPause = true;
    bool editorMode = true;
    float cameraSpeed{0.05f};
    float cameraRotateSpeed{0.1f};
    double mouseSpeed = 0.05;

    //Might implement frustum bools if time
    bool bTestFrustumRight = true;
    bool bTestFrustumLeft = true;
    bool bTestFrustumNear = true;
    bool bTestFrustumFar = true;
    bool bTestFrustumBottom = false;
    bool bTestFrustumTop = false;

    /**Other**/
    gsl::Vector3D thirdPersonCameraPos;
    gsl::Vector3D inFrontOfPlayer;


protected:
    //QWindow which RenderSystem inherits from has Keyboard and mouse function events
    //RenderSystem notifies system whenever an input is pressed
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mousePicking(QMouseEvent *event);
};

#endif // SYSTEM_H

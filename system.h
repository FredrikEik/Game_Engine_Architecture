#ifndef SYSTEM_H
#define SYSTEM_H

#include <QElapsedTimer>
#include <QJSEngine>
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

class System : public QObject, protected QOpenGLFunctions_4_1_Core
{
public:
    System(MainWindow *mw, RenderSystem *rs);
    void init();

    /**Level**/
    void saveLevel();
    void loadLevel();
    void clearLevel();
    void resetLevel();

    /**Toggle between play and editor mode**/
    void toggleEditorMode();

    /**Collision**/
    /*AABB vs AABB Collision*/
    bool isColliding(BoxCollisionComponent &Box1, BoxCollisionComponent &Box2);
    /*AABB vs Sphere Collision*/
    bool isColliding(BoxCollisionComponent &Box, SphereCollisionComponent &Sphere);
    /*Sphere vs Frustum Collision*/
    bool isCollidingWithFrustum(SphereCollisionComponent &Sphere);

    /**Input**/
    void handleInput();

    /**Camera**/
    void setCameraSpeed(float value);

    /**Create object**/
    void createObject(std::string objectName);

    /**Setters**/
    void* setRenderSystem(RenderSystem* rs) {renderSystem = rs;}

    /**Getters**/
    InputComponent* getInput() {return input;}
    Camera* getEditorCamera() {return editorCamera;}
    Camera* getPlayCamera() {return playCamera;}
    float getCameraSpeed() {return cameraSpeed;}
    float getCameraRotateSpeed() {return cameraRotateSpeed;}
    bool getEditorMode() {return editorMode;}

    /**Public for RenderSystems sake**/
    std::vector<GameObject*> gameObjects;
    GameObject* helperObject = nullptr;
    MeshComponent *helperObjectMesh= nullptr;
    QElapsedTimer timeStart;

    /**Sound**/
    SoundSource* clickSound{};
    SoundSource* marioSound{};
    SoundSource* videoGameLandSound{};
    SoundSource* videoGameLandSound2{};

private:
    void initMeshes();
    void initObjects();
    void initTextures();
    void initSounds();

    /**Systems**/
    RenderSystem* renderSystem = nullptr;
    MainWindow* mainWindow = nullptr;
    Factory* factory = nullptr;
    QTimer *gameLoopTimer = nullptr;
    Level level;
    QJSEngine scriptEngine;

    /**Objects**/
    Camera* playCamera = nullptr;
    Camera* editorCamera = nullptr;
    Player* player = nullptr;
    Skybox* skybox = nullptr;
    TriangleSurface* triangleSurface = nullptr;

    /**Input**/
    InputComponent* input;

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

private slots:
    void gameLoop();
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

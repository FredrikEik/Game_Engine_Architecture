#ifndef COREENGINE_H
#define COREENGINE_H

#include <QObject>
#include "input.h"

#include "ResourceManager.h"
#include "soundmanager.h"

#include "GameObject.h"
#include "renderwindow.h"
#include "camera.h"

class QTimer;
class SoundManager;
class ResourceManager;
class RenderWindow;
class Camera;


class CoreEngine : public QObject
{
    Q_OBJECT
public:
    CoreEngine(RenderWindow *renderWindow);

    static CoreEngine *getInstance();

    void SetUpScene();
    void HandleInput();

    Input mInput;

private slots:
    void GameLoop();


private:

    static CoreEngine* mInstance;

    QTimer *mGameLoopTimer{nullptr};
    SoundManager *mSoundManager{nullptr};
    ResourceManager* mResourceManager{nullptr};
    RenderWindow *mRenderWindow{nullptr};
    Camera *mEditorCamera{nullptr};
    Camera *mGameCamera{nullptr};



};

#endif // COREENGINE_H

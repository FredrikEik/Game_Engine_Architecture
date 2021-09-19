#ifndef COREENGINE_H
#define COREENGINE_H

//Forward declarations
class ResourceManager;
class SoundSystem;
class RenderWindow;

class Camera;

class CoreEngine
{
public:
    CoreEngine(RenderWindow *renderWindowIn);

    //TODO:
    //Read and load all assets at start
    //Could read in list of all assets present, and only load on demand <<--- do this
    //List will be used in Editor GUI

    //Handle input

    //Handle GameLoop

    ///Not singleton now, but can use this function to get Engine instance
    static CoreEngine* getInstance();

    void togglePlayMode(bool shouldPlay);

    void setUpScene();

    ResourceManager *mResourceManager{nullptr};
    SoundSystem *mSoundSystem{nullptr};
    RenderWindow *mRenderWindow{nullptr};

    Camera *mGameCamera{nullptr};
    Camera *mEditorCamera{nullptr};

    bool isPlaying{false};  //is the game playing?

private:
    static CoreEngine* mInstance;   //Not singleton, but can use static getInstance()

};

#endif // COREENGINE_H

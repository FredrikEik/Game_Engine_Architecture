#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <vector>

#include "gameobject.h"



class GameEngine
{
public:
    GameEngine();
    void Coreinit();

    static GameEngine* getInstance()
    {
        if(!mInstance)
        {
            mInstance = new GameEngine();
        }
        return mInstance;
    }
    void setRenderPointer(class RenderWindow* temp);

private:
    class ResourceManager *mResourceManager{nullptr};
    //std::vector<GameObject*> mGameObjects;

    static GameEngine* mInstance;
    class RenderWindow* mRenderwindow{nullptr};
};

#endif // GAMEENGINE_H

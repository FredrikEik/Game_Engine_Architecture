#ifndef PHYSICSHANDLER_H
#define PHYSICSHANDLER_H

//Forward declaration
class GameObject;
class RenderSystem;
struct MeshData;
class GameObjectManager;

class PhysicsHandler
{
public:
    PhysicsHandler(RenderSystem *renderSystemIn);

    void movePhysicsObject(std::vector<GameObject*> mGameObjects);

    RenderSystem *mRenderSystem{nullptr};
    GameObjectManager *mGameObjectManager{nullptr};

private:
    class Logger *mLogger{nullptr};
};

#endif // PHYSICSHANDLER_H

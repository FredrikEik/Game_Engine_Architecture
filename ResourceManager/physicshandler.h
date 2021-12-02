#ifndef PHYSICSHANDLER_H
#define PHYSICSHANDLER_H

//Forward declaration
class GameObject;
class RenderSystem;
class GameObjectManager;

class PhysicsHandler
{
public:
    PhysicsHandler(RenderSystem *renderSystemIn);

    void movePhysicsObject(std::vector<GameObject*> mGameObjects, bool simulatePhysics, int numberOfSimulatedBalls);

    RenderSystem *mRenderSystem{nullptr};
    GameObjectManager *mGameObjectManager{nullptr};

    gsl::Vector3D velocity{0.0f, 0.0f, 0.0f};
    gsl::Vector3D gravity{0.0f, 9.8f, 0.0f};

private:
    class Logger *mLogger{nullptr};
};

#endif // PHYSICSHANDLER_H

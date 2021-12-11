#ifndef PHYSICSHANDLER_H
#define PHYSICSHANDLER_H

//Forward declaration
class GameObject;
class GameObjectManager;

/**
  The PhsysicsHandler is from the Visualization & simulation course.
  Mostly not used now, but contains function for simulating balls rolling down meshes.
  Integrated with the GameObjectManager to get references to the gameobjects needed for the simualtion.
 */

class PhysicsHandler
{
public:
    PhysicsHandler();

    void movePhysicsObject(std::vector<GameObject*> mGameObjects, bool simulatePhysics, int numberOfSimulatedBalls);

    GameObjectManager *mGameObjectManager{nullptr};

    gsl::Vector3D gravity{0.0f, 9.8f, 0.0f};

private:
    class Logger *mLogger{nullptr};
};

#endif // PHYSICSHANDLER_H

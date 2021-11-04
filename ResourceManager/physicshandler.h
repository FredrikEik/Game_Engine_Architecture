#ifndef PHYSICSHANDLER_H
#define PHYSICSHANDLER_H

//Forward declaration
class GameObject;
class RenderSystem;
class MeshHandler;


class PhysicsHandler
{
public:
    PhysicsHandler(RenderSystem *renderSystemIn);

    void movePhysicsObject(std::vector<GameObject*> mGameObjects);

    RenderSystem *mRenderSystem{nullptr};
    MeshHandler* mMeshHandler{nullptr};

//    std::map<std::string, unsigned int> mMeshMap; //Trying to get access to vertex data in physicssimulations
//    std::vector<MeshData> mMeshes;

private:
    class Logger *mLogger{nullptr};
};

#endif // PHYSICSHANDLER_H

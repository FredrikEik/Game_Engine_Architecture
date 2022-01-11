#include "ResourceManager/physicshandler.h"
#include "gameobject.h"

#include "coreengine.h"
#include "gameobjectmanager.h"

PhysicsHandler::PhysicsHandler()
{
    mLogger = Logger::getInstance();
    mGameObjectManager = &GameObjectManager::getInstance();
}

void PhysicsHandler::movePhysicsObject(std::vector<GameObject*> mGameObjects, bool simulatePhysics)
{
    if(simulatePhysics == true) //Only simulate Physics when QAction is pressed in MainWindow
    {
        //Get the map of GameObjects and their meshdata from CoreEngine.
        std::map<std::string, unsigned int> GameObjectMap = CoreEngine::getInstance()->mGameObjectManager->mMeshHandler->get_mMeshMap();
        std::vector<MeshData> GameObjectMeshData =  CoreEngine::getInstance()->mGameObjectManager->mMeshHandler->get_mMeshes();

        //Search for "searchGroundName" and create a gameobject using it.
        std::string searchGroundName = "LasGround";
        GameObject groundObject;
        std::vector<Vertex> triangleVertices;

        //Get the details of the plane the physicsobject is going to interact with.
        for(int i = 0; i < mGameObjects.size(); i++)
        {
            if(mGameObjects[i]->mName == searchGroundName) //If string name matches - copy all the info from mGameObjects into groundObject
            {
                groundObject.mName = mGameObjects[i]->mName;
                groundObject.mMesh = mGameObjects[i]->mMesh;
                groundObject.mMaterial = mGameObjects[i]->mMaterial;
                groundObject.mTransform = mGameObjects[i]->mTransform;
                groundObject.mPhysicsComponent = mGameObjects[i]->mPhysicsComponent;

                auto placement = GameObjectMap.find("Data"); //with the name found, find the index in the gameObjectMap

                triangleVertices = GameObjectMeshData[placement->second].get_MeshData_mVertices(); //Get the vertices, the "second" value from the GameObjectMeshData
            }
        }

    //Find the vector3d position of the ground
    gsl::Vector3D groundPosition3D = {groundObject.mTransform->mMatrix.getPosition()};

//    Get the details of the ball
    std::string searchGameName{0};
    GameObject physicsBall;
    gsl::Vector3D ballPosition3D;

    searchGameName = "RulleBall"; //Search for gameobject named:

    for (int i = 0; i < mGameObjects.size(); i++)
    {
        if(mGameObjects[i]->mName == searchGameName) //If string name matches - copy all the info from mGameObjects into physicsBall
        {
            physicsBall.mName = mGameObjects[i]->mName;
            physicsBall.mMesh = mGameObjects[i]->mMesh;
            physicsBall.mMaterial = mGameObjects[i]->mMaterial;
            physicsBall.mTransform = mGameObjects[i]->mTransform;
            physicsBall.mPhysicsComponent = mGameObjects[i]->mPhysicsComponent;

            //Find the Vector3D position of the ball
            ballPosition3D = physicsBall.mTransform->mMatrix.getPosition();
        }
    }

    gsl::Vector3D baryCoordinates;

    //Search through all trianglevertices and get barycentric based on ball coordinates.
    for (int i = 0; i < triangleVertices.size()-2; i += 3) //Cycle through trianglevertices three by three.
    {
        //Barycentric Coordinate function - https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
        gsl::Vector3D v0 = triangleVertices[i+1].mXYZ - triangleVertices[i].mXYZ,
                      v1 = triangleVertices[i+2].mXYZ - triangleVertices[i].mXYZ,
                      v2 = ballPosition3D - triangleVertices[i].mXYZ;

        float den = (v0.x * v1.y) - (v1.x * v0.y);

        baryCoordinates.x = (v2.x * v1.y - v1.x * v2.y) / den;
        baryCoordinates.y = (v0.x * v2.y - v2.x * v0.y) / den;
        baryCoordinates.z = 1.0f - baryCoordinates.x - baryCoordinates.y;

        qDebug() << baryCoordinates;

        //If barycentric is 0 or above, current closest triangle have been found.
        if (baryCoordinates.x >= 0.0f && baryCoordinates.y >= 0.0f && baryCoordinates.z >= 0.0f)
        {
            qDebug() << baryCoordinates;

            //Normal of triangle is calculated.
            gsl::Vector3D triangleNormal;
            triangleNormal = (triangleVertices[i+1].mXYZ - triangleVertices[i].mXYZ) ^
                             (triangleVertices[i+1].mXYZ - triangleVertices[i+2].mXYZ);
            triangleNormal.normalize();

            //Move the ball
            gsl::Vector3D acceleration;
            gsl::Vector3D velocity;
            gsl::Vector3D newBallPosition;

            acceleration = gravity * 0.05f ^ triangleNormal ^ gsl::Vector3D(0, triangleNormal.y, 0);
            velocity = velocity + acceleration * 0.17f;

            newBallPosition = ballPosition3D + velocity;

            //Only needed when simulating a single ball
            newBallPosition.y = (baryCoordinates.x * triangleVertices[i].mXYZ.y +
                                 baryCoordinates.y * triangleVertices[i+1].mXYZ.y + 0.25f +
                                 baryCoordinates.z * triangleVertices[i+2].mXYZ.y);

            physicsBall.mTransform->mMatrix.setPosition(newBallPosition.x, newBallPosition.y, newBallPosition.z);
        }
    }
        if (baryCoordinates.x < 0 || baryCoordinates.x < 0 || baryCoordinates.z < 0.0f) //if no triangle is found, make the ball fall.
        {
            qDebug() << "No triangle found";

            gsl::Vector3D acceleration;
            gsl::Vector3D velocity;
            gsl::Vector3D newBallPosition;

            acceleration = (-gravity * 0.01f);

            velocity = velocity + (acceleration * 0.17f);

            newBallPosition = ballPosition3D + velocity;

            physicsBall.mTransform->mMatrix.setPosition(newBallPosition.x, newBallPosition.y, newBallPosition.z);
        }
    }//End of simulatePhysics-bool loop
}



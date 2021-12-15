#include "ResourceManager/physicshandler.h"
#include "gameobject.h"

#include "coreengine.h"
#include "gameobjectmanager.h"

PhysicsHandler::PhysicsHandler()
{
    mLogger = Logger::getInstance();
    mGameObjectManager = &GameObjectManager::getInstance();
}

void PhysicsHandler::movePhysicsObject(std::vector<GameObject*> mGameObjects, bool simulatePhysics, const int numberOfSimulatedBalls)
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
    GameObject physicsBall[51];
    gsl::Vector3D ballPosition3D[51];

    //Trying to get each ball their own coordinates to go by, this works.
    gsl::Vector3D v0[51], v1[51], v2[51];
    gsl::Vector3D baryCoordinates[51];
    gsl::Vector3D triangleNormal[51];
    float den[51];
    gsl::Vector3D acceleration[51];
    gsl::Vector3D velocity[51];
    gsl::Vector3D newBallPosition[51];

    searchGameName = "RulleBall";

    //Run through rest of code per ball
//    for (int ball = 0; ball <= numberOfSimulatedBalls; ball++)
//    {
        int ball = 0;
        for (int i = 0; i < mGameObjects.size(); i++)
        {
//            searchGameName = "RollingBall_" + std::to_string(i);

            if(mGameObjects[i]->mName == searchGameName) //If string name matches - copy all the info from mGameObjects into physicsBall
            {
                physicsBall[ball].mName = mGameObjects[i]->mName;
                physicsBall[ball].mMesh = mGameObjects[i]->mMesh;
                physicsBall[ball].mMaterial = mGameObjects[i]->mMaterial;
                physicsBall[ball].mTransform = mGameObjects[i]->mTransform;
                physicsBall[ball].mPhysicsComponent = mGameObjects[i]->mPhysicsComponent;

                //Find the Vector3D position of the ball
                ballPosition3D[ball] = physicsBall[ball].mTransform->mMatrix.getPosition();
            }
        }
//qDebug() << triangleVertices.size();
        //Search through all trianglevertices and get barycentric based on ball coordinates.
        for (int i = 0; i < triangleVertices.size()-2; i += 3) //Cycle through trianglevertices three by three.
        {
            //Barycentric Coordinate function - https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
            v0[ball] = triangleVertices[i+1].mXYZ - triangleVertices[i].mXYZ,
            v1[ball] = triangleVertices[i+2].mXYZ - triangleVertices[i].mXYZ,
            v2[ball] = ballPosition3D[ball] - triangleVertices[i].mXYZ;

            den[ball] = (v0[ball].x * v1[ball].y) - (v1[ball].x * v0[ball].y);
            baryCoordinates[ball].x = (v2[ball].x * v1[ball].y - v1[ball].x * v2[ball].y) / den[ball];
            baryCoordinates[ball].y = (v0[ball].x * v2[ball].y - v2[ball].x * v0[ball].y) / den[ball];
            baryCoordinates[ball].z = 1.0f - baryCoordinates[ball].x - baryCoordinates[ball].y;

//    qDebug() << baryCoordinates[0];

            //If barycentric is 0 or above, current closest triangle have been found.
            if (baryCoordinates[ball].x >= 0.0f && baryCoordinates[ball].y >= 0.0f && baryCoordinates[ball].z >= 0.0f)
            {
                //Normal of triangle is calculated.
                triangleNormal[ball] = (triangleVertices[i+1].mXYZ - triangleVertices[i].mXYZ) ^
                                       (triangleVertices[i+1].mXYZ - triangleVertices[i+2].mXYZ);
                triangleNormal[ball].normalize();

                //Move the ball
                acceleration[ball] = gravity * 0.05f ^ triangleNormal[ball] ^ gsl::Vector3D(0, triangleNormal[ball].y, 0);
                velocity[ball] = velocity[ball] + acceleration[ball] * 0.17f;

                newBallPosition[ball] = ballPosition3D[ball] + velocity[ball];

                physicsBall[ball].mTransform->mMatrix.setPosition(newBallPosition[ball].x, newBallPosition[ball].y, newBallPosition[ball].z);
            }
        }
            if (baryCoordinates[ball].x < 0 || baryCoordinates[ball].x < 0 || baryCoordinates[ball].z < 0.0f) //if no triangle is found, make the ball fall.
            {
                acceleration[ball] = (-gravity * 0.01f);

                velocity[ball] = velocity[ball] + (acceleration[ball] * 0.17f);

                newBallPosition[ball] = ballPosition3D[ball] + velocity[ball];

                physicsBall[ball].mTransform->mMatrix.setPosition(newBallPosition[ball].x, newBallPosition[ball].y, newBallPosition[ball].z);
            }
//        }//End of for loop for the 51 balls
    }//End of simulatePhysics-bool loop
}

////Only needed when simulating a single ball
//newBallPosition[ball].y = (baryCoordinates[ball].x * triangleVertices[i].mXYZ.y +
//                           baryCoordinates[ball].y * triangleVertices[i+1].mXYZ.y + 0.25f +
//                           baryCoordinates[ball].z * triangleVertices[i+2].mXYZ.y);

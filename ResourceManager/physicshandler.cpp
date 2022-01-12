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
        std::vector<GLuint> triangleIndices;

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
                triangleIndices = GameObjectMeshData[placement->second].get_MeshData_mIndices();
            }
        }
    //Find the vector3d position of the ground
    gsl::Vector3D groundPosition3D = groundObject.mTransform->mMatrix.getPosition();

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
        }
    }

    //Find the Vector3D position of the ball
    ballPosition3D = physicsBall.mTransform->mMatrix.getPosition();

    gsl::Vector3D baryCoordinates;

    //Search through all trianglevertices and get barycentric based on ball coordinates.
    for (int i = 0; i < triangleIndices.size()-2; i += 3) //Cycle through trianglevertices three by three.
    {
        //Get corners of triangle nr i.
//        gsl::Vector3D p0 = gsl::Vector3D(triangleVertices[triangleIndices[i]].mXYZ), //Hmm, something seems odd here. does not look like three points that make up a triangle.
//                      p1 = gsl::Vector3D(triangleVertices[triangleIndices[i+1]].mXYZ),
//                      p2 = gsl::Vector3D(triangleVertices[triangleIndices[i+2]].mXYZ);

//        //Get Barycentric cordinates based on ball given triangle i.
//        baryCoordinates = ballPosition3D.barycentricCoordinates(p0, p1, p2);

        //Barycentric Coordinate function - https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
//        for(int j = 0; j < triangleIndices.size()-2; j+= 3)
//        {
        gsl::Vector3D v0, v1, v2;
        //By searching through indices, i can access values outside of vertices-range. no good.
        v0 = triangleVertices[triangleIndices[i+1]].mXYZ - triangleVertices[triangleIndices[i]].mXYZ;
        v1 = triangleVertices[triangleIndices[i+2]].mXYZ - triangleVertices[triangleIndices[i]].mXYZ;
        v2 = ballPosition3D - triangleVertices[triangleIndices[i]].mXYZ;

        float den = (v0.x * v1.y) - (v1.x * v0.y);

        baryCoordinates.x = (v2.x * v1.y - v1.x * v2.y) / den;
        baryCoordinates.y = (v0.x * v2.y - v2.x * v0.y) / den;
        baryCoordinates.z = 1.0f - baryCoordinates.x - baryCoordinates.y;

    //        qDebug() << den;
            qDebug() << baryCoordinates;
//        }
        //If barycentric is 0 or above, current closest triangle have been found.
        if (baryCoordinates.x >= 0.0f && baryCoordinates.y >= 0.0f && baryCoordinates.z >= 0.0f)
        {
            qDebug() << baryCoordinates;

            //Normal of triangle is calculated.
            gsl::Vector3D triangleNormal;
            triangleNormal = (triangleVertices[triangleIndices[i+1]].mXYZ - triangleVertices[triangleIndices[i]].mXYZ) ^
                             (triangleVertices[triangleIndices[i+1]].mXYZ - triangleVertices[triangleIndices[i+2]].mXYZ);
            triangleNormal.normalize();

            //Move the ball
            gsl::Vector3D acceleration;
            acceleration = gravity * 0.05f ^ triangleNormal ^ gsl::Vector3D(0, triangleNormal.y, 0);
            qDebug() << acceleration; //Answer to question three on 2021 exam.

            gsl::Vector3D velocity;
            velocity = velocity + acceleration * 0.17f;

            gsl::Vector3D newBallPosition;
            newBallPosition = ballPosition3D + velocity;

            //Only needed when simulating a single ball
            newBallPosition.y = (baryCoordinates.x * triangleVertices[triangleIndices[i]].mXYZ.y +
                                 baryCoordinates.y * triangleVertices[triangleIndices[i+1]].mXYZ.y + 0.25f +
                                 baryCoordinates.z * triangleVertices[triangleIndices[i+2]].mXYZ.y);

            physicsBall.mTransform->mMatrix.setPosition(newBallPosition.x, newBallPosition.y, newBallPosition.z);
        }
    }
        if (baryCoordinates.x < 0 || baryCoordinates.x < 0 || baryCoordinates.z < 0.0f) //if no triangle is found, make the ball fall.
        {
//            qDebug() << "No triangle found";

            gsl::Vector3D acceleration;
            gsl::Vector3D velocity;
            gsl::Vector3D newBallPosition;

            acceleration = (-gravity * 0.01f);

            velocity = velocity + (acceleration * 0.17f);

            newBallPosition = ballPosition3D + velocity;

            physicsBall.mTransform->mMatrix.setPosition(newBallPosition.x, newBallPosition.y, newBallPosition.z);
        }
    }//End of simulatePhysics-bool check
}



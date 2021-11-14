#include "ResourceManager/physicshandler.h"
#include "gameobject.h"
#include "rendersystem.h"
#include "coreengine.h"
#include "gameobjectmanager.h"

PhysicsHandler::PhysicsHandler(RenderSystem *renderSystemIn) : mRenderSystem {renderSystemIn}
{
    mLogger = Logger::getInstance();
    mGameObjectManager = &GameObjectManager::getInstance();
}

void PhysicsHandler::movePhysicsObject(std::vector<GameObject*> mGameObjects)
{
//Get the map of GameObjects and their meshdata from CoreEngine.
    std::map<std::string, unsigned int> GameObjectMap = CoreEngine::getInstance()->mGameObjectManager->mMeshHandler->get_mMeshMap();
    std::vector<MeshData> GameObjectMeshData =  CoreEngine::getInstance()->mGameObjectManager->mMeshHandler->get_mMeshes();


//Search for the gameobject called "name" and create a gameobject using it.
    std::string searchGameName = "CrookedTriangleSurface";
    GameObject groundObject;
    std::vector<Vertex> triangleVertices;



////Get the details of the plane the physicsobject is going to interact with.
    for(int i = 0; i < mGameObjects.size(); i++)
    {
        if(mGameObjects[i]->mName == searchGameName) //If string name matches - copy all the info from mGameObjects into groundObject
        {
            groundObject.mName = mGameObjects[i]->mName;
            groundObject.mMesh = mGameObjects[i]->mMesh;
            groundObject.mMaterial = mGameObjects[i]->mMaterial;
            groundObject.mTransform = mGameObjects[i]->mTransform;
            groundObject.mPhysicsComponent = mGameObjects[i]->mPhysicsComponent;

            auto placement = GameObjectMap.find(searchGameName +".obj");

            triangleVertices = GameObjectMeshData[placement->second].get_MeshData_mVertices();
//            qDebug() << "searchGameName" << "found";
            break;
        }
//        if (i == mGameObjects.size()-1) //if entirety of gameobjects is searched and not found, the game-engine crashes trying to log two strings.
//        {
//            mLogger->logText("No gameobject named" + name, LColor::LOG);
//        }
    }



////Get the details of the ball
    searchGameName = "RollingBall";
    GameObject physicsBall;

    for(int i = 0; i < mGameObjects.size(); i++)
    {
        if(mGameObjects[i]->mName == searchGameName) //If string name matches - copy all the info from mGameObjects into physicsBall
        {
            physicsBall.mName = mGameObjects[i]->mName;
            physicsBall.mMesh = mGameObjects[i]->mMesh;
            physicsBall.mMaterial = mGameObjects[i]->mMaterial;
            physicsBall.mTransform = mGameObjects[i]->mTransform;
            physicsBall.mPhysicsComponent = mGameObjects[i]->mPhysicsComponent;
            break;
        }
    }



//Find the vector3d position of the ball
    gsl::Vector3D ballPosition3D = {physicsBall.mTransform->mMatrix.getPosition()};
//    qDebug() << "Ballposition3d:           " << ballPosition3D;


//Find the vector3d position of the ground
    gsl::Vector3D groundPosition3D = {groundObject.mTransform->mMatrix.getPosition()};
//    qDebug() << "Groundposition:           " <<groundPosition3D;


////Find the distance between the balls position and vertices of the trianglesurface
//    std::vector<gsl::Vector3D> distanceBetweenBallAndVert; //Converting to a vector of vector3Ds for scalability

//    for (int i = 0; i < triangleVertices.size(); i++)
//    {
//        distanceBetweenBallAndVert.push_back(ballPosition3D - (triangleVertices[i].mXYZ + groundPosition3D));

        //if the distance is a negative number, flip it. This makes sure that the lowest physical distance is selected, not the lowest number.
//        if (distanceBetweenBallAndVert[i].x < 0)
//                { distanceBetweenBallAndVert[i].x *= -1.0f; }

//        if (distanceBetweenBallAndVert[i].y < 0)
//                { distanceBetweenBallAndVert[i].y *= -1.0f; }

//        if (distanceBetweenBallAndVert[i].z < 0)
//                { distanceBetweenBallAndVert[i].z *= -1.0f; }

//        qDebug() << "Triangle Vert nr        " << i+1  << triangleVertices[i].mXYZ + groundPosition3D;
//        qDebug() << "Distance ball to vert nr" << i+1  << distanceBetweenBallAndVert[i];
//        if (i == 2)
//        {
//            qDebug() << Qt::endl;
//            qDebug() << "Ballposition3d:           " << ballPosition3D;
//        }
//        qDebug() << distanceBetweenBallAndVert[i];
//    }



////Keep the three closest points
//    gsl::Vector3D closestTrianglePoints[3]; //Used to store the three points of the closest triangle
//    std::vector<gsl::Vector3D> comparePoint; //Compare the distance between ball and vert for the different triangles

//    //Sum the distances for the first comparison between triangles
//    for (int i = 0; i < distanceBetweenBallAndVert.size(); i+=3)
//    {
//        comparePoint.push_back(distanceBetweenBallAndVert[i] + distanceBetweenBallAndVert[i+1] + distanceBetweenBallAndVert[i+2]);
//    }
//    qDebug() << "comparePoint size" << comparePoint.size();
//    qDebug() << "Distance to triangle" << 1 << comparePoint[0];
//    qDebug() << "Distance to triangle" << 2 << comparePoint[1];

    //Compare the sum of distances, pick the triangle which is closest.
//    for (int i = 1; i < comparePoint.size(); i++) ////This is as far as i currenlty am with making the code work for arbitrary triangle count meshes.
//    {
////        qDebug() << i;
//        if (comparePoint[i-1].x + comparePoint[i-1].y + comparePoint[i-1].z <= comparePoint[i].x + comparePoint[i].y + comparePoint[i].z)
//        {
//            closestTrianglePoints[0] = triangleVertices[0].mXYZ;
//            closestTrianglePoints[1] = triangleVertices[1].mXYZ;
//            closestTrianglePoints[2] = triangleVertices[2].mXYZ;
////            qDebug() << "First triangle closest and its 3 corners stored";
//        }
//            else
//        {
//            closestTrianglePoints[0] = triangleVertices[3].mXYZ;
//            closestTrianglePoints[1] = triangleVertices[4].mXYZ;
//            closestTrianglePoints[2] = triangleVertices[5].mXYZ;
////            qDebug() << "Second triangle closest and its 3 corners stored";
//        }
//    }

    gsl::Vector3D barycoordinates;
    int triangleNr;

    for (int i = 0; i < triangleVertices.size()-2; i += 3) //search through all trianglevertices and get their barycentric coordinates.
    {
        gsl::Vector3D v0 = (triangleVertices[i+1].mXYZ + groundPosition3D) - (triangleVertices[i].mXYZ + groundPosition3D),
                      v1 = (triangleVertices[i+2].mXYZ + groundPosition3D) - (triangleVertices[i].mXYZ + groundPosition3D),
                      v2 = ballPosition3D - (triangleVertices[i].mXYZ + groundPosition3D);

            float den = (v0.x * v1.z) - (v1.x * v0.z);
            barycoordinates.x = (v2.x * v1.z - v1.x * v2.z) / den;
            barycoordinates.z = (v0.x * v2.z - v2.x * v0.z) / den;
            barycoordinates.y = 1.0f - barycoordinates.x - barycoordinates.z;

//            qDebug() << "Barycentric coordinates" << i << barycoordinates;
            if (barycoordinates.x > 0.0f && barycoordinates.y > 0.0f && barycoordinates.z > 0.0f) //If barycentric is above 0, we have found a triangle.
            {
                triangleNr = i;
                qDebug() << "Triangle found" << (triangleNr/3)+1;
                break; //Break out of the for loop, the triangle is found.
            }
    }

    if (barycoordinates.x > 0.0f && barycoordinates.y > 0.0f && barycoordinates.z > 0.0f)
    {
            gsl::Vector3D tempNormal[2];
            gsl::Vector3D triangleNormal;

            tempNormal[0] = triangleVertices[triangleNr+1].mXYZ - triangleVertices[triangleNr].mXYZ;
            tempNormal[1] = triangleVertices[triangleNr+2].mXYZ - triangleVertices[triangleNr].mXYZ;
            triangleNormal.x = tempNormal[0].y * tempNormal[1].z - tempNormal[0].z * tempNormal[1].y;
            triangleNormal.y = tempNormal[0].z * tempNormal[1].x - tempNormal[0].x * tempNormal[1].z;
            triangleNormal.z = tempNormal[0].x * tempNormal[1].y - tempNormal[0].y * tempNormal[1].x;

//                    qDebug() << "Triangle normal is:" << triangleNormal.x << triangleNormal.y << triangleNormal.z;
            triangleNormal.normalize();
//                    qDebug() << "Normalized Triangle normal is:" << triangleNormal.x << triangleNormal.y << triangleNormal.z;

            ////Move the ball
            gsl::Vector3D acceleration = gravity * 0.01f ^ triangleNormal ^ gsl::Vector3D(0, triangleNormal.y, 0);

            velocity = velocity + (acceleration * 0.17f);

            gsl::Vector3D newBallPosition = ballPosition3D + velocity;
            float ballYOffset = 0.15f;

            newBallPosition.y = (barycoordinates.x * triangleVertices[triangleNr+0].mXYZ.y +
                                 barycoordinates.y * triangleVertices[triangleNr+1].mXYZ.y +
                                 barycoordinates.z * triangleVertices[triangleNr+2].mXYZ.y);

                                                            //With the -1 i can choose which triangle should behave correctly -.-
            physicsBall.mTransform->mMatrix.setPosition(newBallPosition.x, (newBallPosition.y/* * -1*/) + ballYOffset, newBallPosition.z);
            qDebug() << "ball is moving towards" << acceleration;
    }
//    if (barycoordinates.x < 0.0f || barycoordinates.y < 0.0f || barycoordinates.z < 0.0f)
//    {
//            gsl::Vector3D acceleration = (-gravity * 0.01f);

//            velocity = velocity + (acceleration * 0.17f);

//            gsl::Vector3D newBallPosition = ballPosition3D + velocity;

//            physicsBall.mTransform->mMatrix.setPosition(newBallPosition.x, newBallPosition.y, newBallPosition.z);
//            qDebug() << "Ball is falling";
//    }



////Getting barycentric coordinates of ball position and closest triangle
//    gsl::Vector3D barycoordinates;
//    gsl::Vector3D ballPosition2D = physicsBall.mTransform->mMatrix.getPosition();
//    ballPosition2D.y = 0;

//    Using Dags barycentric cordinate function from "rulleball" gives un-expected results?
//    The barycentric coordinates does not add up to 1, and goes higher the longer away from plane they are?
//    barycoordinates = ballPosition3D.barycentricCoordinates(closestTrianglePoint[0], closestTrianglePoint[1], closestTrianglePoint[2]);


    //Barycentric function found here adds up to 1, and gives negative value when off triangle
    //https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
    //This only works on the "crooked" trianglesurface? To change surface, its initializing in coreengine lines 150 & 154, and searchGameName in this cpp.
//    gsl::Vector3D v0 = closestTrianglePoints[1] - closestTrianglePoints[0],
//                  v1 = closestTrianglePoints[2] - closestTrianglePoints[0],
//                  v2 = ballPosition3D - closestTrianglePoints[0];

//    float den = (v0.x * v1.z) - (v1.x * v0.z); //Tried replacing "y" coordinate with "z" to match engine, but gives weird result.
//    barycoordinates.x = (v2.x * v1.z - v1.x * v2.z) / den;
//    barycoordinates.z = (v0.x * v2.z - v2.x * v0.z) / den;
//    barycoordinates.y = 1.0f - barycoordinates.x - barycoordinates.z;

//    qDebug() << "Barycentric coordinates to closest triangle" << barycoordinates;



////Calculating the normalvector of the triangle the ball is on
//    gsl::Vector3D tempNormal[2];
//    gsl::Vector3D triangleNormal;

//    tempNormal[0] = closestTrianglePoints[1] - closestTrianglePoints[0];
//    tempNormal[1] = closestTrianglePoints[2] - closestTrianglePoints[0];
//    triangleNormal.x = tempNormal[0].y * tempNormal[1].z - tempNormal[0].z * tempNormal[1].y;
//    triangleNormal.y = tempNormal[0].z * tempNormal[1].x - tempNormal[0].x * tempNormal[1].z;
//    triangleNormal.z = tempNormal[0].x * tempNormal[1].y - tempNormal[0].y * tempNormal[1].x;

//    qDebug() << "Triangle normal is:" << triangleNormal.x << triangleNormal.y << triangleNormal.z;
//    triangleNormal.normalize();
//    qDebug() << "Normalized Triangle normal is:" << triangleNormal.x << triangleNormal.y << triangleNormal.z;



////Update ball speed across triangle
//    if(barycoordinates.x >= 0.0f && barycoordinates.y >= 0.0f && barycoordinates.z >= 0.0f)
//    {
//        gsl::Vector3D acceleration = gravity * 0.01f ^ triangleNormal ^ gsl::Vector3D(0, triangleNormal.y, 0);

//        velocity = velocity + (acceleration * 0.17f);

//        gsl::Vector3D newBallPosition = ballPosition3D + velocity;
//        float ballYOffset = 0.4f;

//        newBallPosition.y = (barycoordinates.x * closestTrianglePoints[0].y +
//                             barycoordinates.y * closestTrianglePoints[1].y +
//                             barycoordinates.z * closestTrianglePoints[2].y);

//                                                        //With the -1 i can choose which triangle should behave correctly -.-
//        physicsBall.mTransform->mMatrix.setPosition(newBallPosition.x, (newBallPosition.y/* * -1*/)  + ballYOffset, newBallPosition.z);
//        qDebug() << "ball is moving towards" << acceleration;
//    }
//    if (barycoordinates.x > 1.0f || barycoordinates.y > 1.0f || barycoordinates.z > 1.0f)
//    if (barycoordinates.x < 0.0f || barycoordinates.y < 0.0f || barycoordinates.z < 0.0f) //If the ball isnt on a triangle, fall instead.
//    if ((barycoordinates.x + barycoordinates.y + barycoordinates.z) > 1.0f)
//    else
//    {
//        gsl::Vector3D acceleration = (-gravity * 0.01f);

//        velocity = velocity + (acceleration * 0.17f);

//        gsl::Vector3D newBallPosition = ballPosition3D + velocity;

//        physicsBall.mTransform->mMatrix.setPosition(newBallPosition.x, newBallPosition.y, newBallPosition.z);
//        qDebug() << "Ball is falling";
//    }
}

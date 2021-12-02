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

void PhysicsHandler::movePhysicsObject(std::vector<GameObject*> mGameObjects, bool simulatePhysics, const int numberOfSimulatedBalls)
{
if(simulatePhysics == true)
{
//    qDebug() << "Simulating physics";

//Get the map of GameObjects and their meshdata from CoreEngine.
    std::map<std::string, unsigned int> GameObjectMap = CoreEngine::getInstance()->mGameObjectManager->mMeshHandler->get_mMeshMap();
    std::vector<MeshData> GameObjectMeshData =  CoreEngine::getInstance()->mGameObjectManager->mMeshHandler->get_mMeshes();


//Search for the gameobject called "name" and create a gameobject using it.
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

            auto placement = GameObjectMap.find(searchGroundName);

            triangleVertices = GameObjectMeshData[placement->second].get_MeshData_mVertices();
//            qDebug() << "Gameobject" << QString::fromStdString(searchGameName) << "found"; //Nice way to get from a std::string to qDebug printable string.
//            qDebug() << triangleVertices.size();
            break;
        }
//        if (i == mGameObjects.size()-1) //if entirety of gameobjects is searched and not found, the game-engine crashes trying to log two strings.
//        {
//            mLogger->logText("No gameobject named" + name, LColor::LOG);
//        }
    }

//Find the vector3d position of the ground
    gsl::Vector3D groundPosition3D = {groundObject.mTransform->mMatrix.getPosition()};
//    qDebug() << "Groundposition:           " <<groundPosition3D;


    //Get the details of the ball
    std::string searchGameName {0};
    GameObject physicsBall[51];
    gsl::Vector3D ballPosition3D[51];
    int foundBalls{0};

    //set up variables before entering big for loop
    gsl::Vector3D barycoordinates[51];

//// Run through rest of code per ball
    for (int ball = 0; ball <= numberOfSimulatedBalls; ball++)
    {
        for (int i = 0; i < mGameObjects.size(); i++)
        {
            searchGameName = "RollingBall_" + std::to_string(ball);

            if(mGameObjects[i]->mName == searchGameName) //If string name matches - copy all the info from mGameObjects into physicsBall
            {
//                qDebug() << "ball nr" << ball << "found on gameobject nr" << i;
                foundBalls++;
                physicsBall[ball].mName = mGameObjects[i]->mName;
                physicsBall[ball].mMesh = mGameObjects[i]->mMesh;
                physicsBall[ball].mMaterial = mGameObjects[i]->mMaterial;
                physicsBall[ball].mTransform = mGameObjects[i]->mTransform;
                physicsBall[ball].mPhysicsComponent = mGameObjects[i]->mPhysicsComponent;

                //Find the Vector3D position of the ball
                ballPosition3D[ball] = physicsBall[ball].mTransform->mMatrix.getPosition();
            }
//            if (foundBalls == numberOfSimulatedBalls - 1) //If all balls are found
//            {
//                qDebug() << "All balls found";
//                break; //break the for loop.
//            }
//        }
        }

//    qDebug() << "Ballposition3d:" << ballPosition3D;


        ////Search through all trianglevertices and get their barycentric coordinates.
        for (int i = 0; i < triangleVertices.size()-2; i += 3) //Cycle through trianglevertices three by three.
        {
            //Barycentric Coordinate function - https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
            gsl::Vector3D v0 = triangleVertices[i+1].mXYZ - triangleVertices[i].mXYZ,
                          v1 = triangleVertices[i+2].mXYZ - triangleVertices[i].mXYZ,
                          v2 = ballPosition3D[ball] - triangleVertices[i].mXYZ;

            float den = (v0.x * v1.y) - (v1.x * v0.y);
            barycoordinates[ball].x = (v2.x * v1.y - v1.x * v2.y) / den; //and you cant divide by 0;
            barycoordinates[ball].y = (v0.x * v2.y - v2.x * v0.y) / den;
            barycoordinates[ball].z = 1.0f - barycoordinates[ball].x - barycoordinates[ball].y;

        //If barycentric is 0 or above, current closest triangle have been found.
        if (barycoordinates[ball].x >= 0.0f && barycoordinates[ball].y >= 0.0f && barycoordinates[ball].z >= 0.0f)
        {
//              qDebug() << "Barycentric coordinates" << barycoordinates;
//              qDebug() << "Barycentric total      " << barycoordinates.x + barycoordinates.y + barycoordinates.z;
//              qDebug() << "Triangle found" << (i/3)+1;

            ////Normal of triangle is calculated.
            gsl::Vector3D triangleNormal;
            triangleNormal = (triangleVertices[i+1].mXYZ - triangleVertices[i].mXYZ) ^
                             (triangleVertices[i+1].mXYZ - triangleVertices[i+2].mXYZ);

//            qDebug() << "Triangle normal is:" << triangleNormal;
            triangleNormal.normalize();
//            qDebug() << "Normalized Triangle is:" << triangleNormal;


            ////Move the ball
            gsl::Vector3D acceleration = gravity * 0.05f ^ triangleNormal ^ gsl::Vector3D(0, triangleNormal.y, 0);
            velocity = velocity + acceleration * 0.17f;

            gsl::Vector3D newBallPosition = ballPosition3D[i] + velocity;
            float ballYOffset = 0.15f;

//            newBallPosition.y = (barycoordinates.x * triangleVertices[i].mXYZ.y +
//                                 barycoordinates.y * triangleVertices[i+1].mXYZ.y + /*ballYOffset +*/
//                                 barycoordinates.z * triangleVertices[i+2].mXYZ.y);

            physicsBall[ball].mTransform->mMatrix.setPosition(newBallPosition.x, (newBallPosition.y), newBallPosition.z);
//            qDebug() << "ball is moving towards " << newBallPosition;
            break; //Break out of the for loop, the triangle is found, its normal calculated and ball position updated.
        }
        }

        if (barycoordinates[ball].x < 0 || barycoordinates[ball].x < 0 || barycoordinates[ball].z < 0.0f) //if no triangle is found, make the ball fall.
        {
            gsl::Vector3D acceleration = (-gravity * 0.01f);

            velocity = velocity + (acceleration * 0.17f);

            gsl::Vector3D newBallPosition = ballPosition3D[ball] + velocity;

            physicsBall[ball].mTransform->mMatrix.setPosition(newBallPosition.x, newBallPosition.y, newBallPosition.z);
//            qDebug() << "Ball is falling";
        }

    }////End of for loop for the 51 balls
} ////End of simulatePhysics-bool loop


////------------Old Method - delivered in compulsery 2 for vis & sim - not scalable to multiple meshes with more than 2 triangles.---------

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

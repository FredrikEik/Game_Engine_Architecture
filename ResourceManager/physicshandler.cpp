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

    //Search for the gameobject called "name" and create a reference to the GameObject the ball will roll on.
    std::string name = "TriangleSurface";
    GameObject groundObject;
    std::vector<Vertex> triangleVertices;

//Get the details of the plane the physicsobject is going to interact with.
    for(int i = 0; i < mGameObjects.size(); i++)
    {
        if(mGameObjects[i]->mName == name) //If string name matches - copy all the info from mGameObjects into groundObject
        {
            groundObject.mName = mGameObjects[i]->mName;
            groundObject.mMesh = mGameObjects[i]->mMesh;
            groundObject.mMaterial = mGameObjects[i]->mMaterial;
            groundObject.mTransform = mGameObjects[i]->mTransform;
            groundObject.mPhysicsComponent = mGameObjects[i]->mPhysicsComponent;

            auto placement = GameObjectMap.find(name +".obj");

            triangleVertices = GameObjectMeshData[placement->second].get_MeshData_mVertices();

            break;
        }
//        if (i == mGameObjects.size()-1) //if entirety of gameobjects is searched and not found, the game-engine crashes trying to log two strings.
//        {
//            mLogger->logText("No gameobject named" + name, LColor::LOG);
//        }
    }
    name.clear();
    //mLogger->logText(std::to_string(groundObject.mMesh->mVertexCount[0])); //How many vertices are there in TriangleSurface?

    //qDebug() << triangleVertices[0].mXYZ;

//Get the details of the ball
    name = "RollingBall";
    GameObject physicsBall;

    for(int i = 0; i < mGameObjects.size(); i++)
    {
        if(mGameObjects[i]->mName == name) //If string name matches - copy all the info from mGameObjects into physicsBall
        {
            physicsBall.mName = mGameObjects[i]->mName;
            physicsBall.mMesh = mGameObjects[i]->mMesh;
            physicsBall.mMaterial = mGameObjects[i]->mMaterial;
            physicsBall.mTransform = mGameObjects[i]->mTransform;
            physicsBall.mPhysicsComponent = mGameObjects[i]->mPhysicsComponent;
            break;
        }
    }
    //mLogger->logText(physicsBall.mName, LColor::LOG);


//Find the vector3d position of the ball
    gsl::Vector3D ballPosition3d = {physicsBall.mTransform->mMatrix.getPosition()};
    //qDebug() << "Ballposition3d: X" << ballPosition3d.x << "Y" << ballPosition3d.y << "Z" << ballPosition3d.z;


//Find the vector3d position of the ground
    gsl::Vector3D groundPosition3d = {groundObject.mTransform->mMatrix.getPosition()};
    //qDebug() << "Groundposition:" << groundObject.mTransform->mMatrix.getPosition();


//Find the distance between the balls position and vertices of the trianglesurface
    gsl::Vector3D distanceBetweenBallAndVert[6]; //using vertices.size() instead of hardcoding 6 would be better.

    for (int i = 0; i < triangleVertices.size(); i++)
    {
        distanceBetweenBallAndVert[i] = triangleVertices[i].mXYZ - ballPosition3d;

        //if the distance is a negative number, flip it. This makes sure that the lowest physical distance is selected, not the lowest number.
        if (distanceBetweenBallAndVert[i].x < 0)
                { distanceBetweenBallAndVert[i].x *= -1.0f; }

        if (distanceBetweenBallAndVert[i].y < 0)
                { distanceBetweenBallAndVert[i].y *= -1.0f; }

        if (distanceBetweenBallAndVert[i].z < 0)
                { distanceBetweenBallAndVert[i].z *= -1.0f; }


//    qDebug() << "Vert nr                 " << i+1 << "X" << physicsBall.mTransform->mMatrix.getPosition().x << "Y" << physicsBall.mTransform->mMatrix.getPosition().y << "Z" << physicsBall.mTransform->mMatrix.getPosition().z;
//    qDebug() << "Distance ball to vert nr" << i+1 << "X" << distanceBetweenBallAndVert[i].x << "Y" << distanceBetweenBallAndVert[i].y << "Z" << distanceBetweenBallAndVert[i].z;
    }



////Keep the three closest points
    gsl::Vector3D closestTrianglePoint[3]; //Used to store the three points of the closest triangle

    closestTrianglePoint[0] =     distanceBetweenBallAndVert[0] + distanceBetweenBallAndVert[1] + distanceBetweenBallAndVert[2];
    gsl::Vector3D comparePoint = distanceBetweenBallAndVert[3] + distanceBetweenBallAndVert[4] + distanceBetweenBallAndVert[5];

    qDebug() << closestTrianglePoint[0].x << closestTrianglePoint[0].y << closestTrianglePoint[0].z;
    qDebug() << comparePoint.x << comparePoint.y << comparePoint.z;

    if (closestTrianglePoint[0].x + closestTrianglePoint[0].y + closestTrianglePoint[0].z <= comparePoint.x + comparePoint.y + comparePoint.z)
    {
        closestTrianglePoint[0] = triangleVertices[0].mXYZ;
        closestTrianglePoint[1] = triangleVertices[1].mXYZ;
        closestTrianglePoint[2] = triangleVertices[2].mXYZ;
        qDebug() << "First triangle closest and its 3 corners stored";
    }
        else
    {
        closestTrianglePoint[0] = triangleVertices[3].mXYZ;
        closestTrianglePoint[1] = triangleVertices[4].mXYZ;
        closestTrianglePoint[2] = triangleVertices[5].mXYZ;
        qDebug() << "Second triangle closest and its 3 corners stored";
    }



//Getting barycentric cordinates of ball position and closest triangle
    gsl::Vector3D ballPosition = physicsBall.mTransform->mMatrix.getPosition(); //Get position of ball in x,y space
    ballPosition.y = 0;
    gsl::Vector3D baryCordinates;
    baryCordinates = ballPosition.barycentricCoordinates(closestTrianglePoint[0], closestTrianglePoint[1], closestTrianglePoint[2]);
    qDebug() << "Barycentric cordinates to closest triangle" << baryCordinates.x << baryCordinates.y << baryCordinates.z;


    //Calculating the normalvector of the triangle the ball is on
    gsl::Vector3D tempNormal[2];
    gsl::Vector3D triangleNormal;

    tempNormal[0] = closestTrianglePoint[1] - closestTrianglePoint[0];
    tempNormal[1] = closestTrianglePoint[2] - closestTrianglePoint[0];
    triangleNormal.x = tempNormal[0].y * tempNormal[1].z - tempNormal[0].z * tempNormal[1].y;
    triangleNormal.y = tempNormal[0].z * tempNormal[1].x - tempNormal[0].x * tempNormal[1].z;
    triangleNormal.z = tempNormal[0].x * tempNormal[1].y - tempNormal[0].y * tempNormal[1].x;

    //qDebug() << "Triangle normal is:" << triangleNormal.x << triangleNormal.y << triangleNormal.z;
    triangleNormal.normalize();
    //qDebug() << "Normalized Triangle normal is:" << triangleNormal.x << triangleNormal.y << triangleNormal.z;


//Update ball speed across triangle
//    if(baryCordinates.x >= 0.0f && baryCordinates.y >= 0.0f && baryCordinates.z >= 0.0f)
//    {
        //Setting up some variables to move ball
//        gsl::Vector3D acceleration = (gravity * 0.001f) ^ triangleNormal ^ gsl::Vector3D(0, 0, triangleNormal.z);
//        velocity = velocity + (acceleration * 0.17f);
//        gsl::Vector3D newBallPosition = physicsBall.mTransform->mMatrix.getPosition() + velocity;
//        float ballZOffset = 0.25f;
//        //ballSpeed = triangleNormal * dt; //Ballspeed, framerate-dependent beacuse DT is set at 0.017 (in theory 16 1/3ms = 60hz)

//        newBallPosition.z = (baryCordinates.x * closestTrianglePoint[0].z +
//                             baryCordinates.y * closestTrianglePoint[0].z +
//                             baryCordinates.z * closestTrianglePoint[0].z);

//        //qDebug() << newBallPosition.z << closestTrianglePoint[0].z;

//        physicsBall.mTransform->mMatrix.translate(newBallPosition.x, newBallPosition.y, newBallPosition.z + ballZOffset); //Based on calculations in either collision or free-fall apply translation to ball.
//        qDebug() << "BaryCordinates before move" << baryCordinates.x << baryCordinates.y << baryCordinates.z;
//        qDebug() << "ball is moving";

//    }
//    else
//    {
//        gsml::Vector3d acceleration = (gravity * 0.001f) *-1 ^ triangleNormal ^ gsml::Vector3d(0, 0, triangleNormal.z);
//        velocity = velocity + (acceleration * 0.17);
//        float ballZOffset = 0.25f;

//        gsml::Vector3d newBallPosition = mMatrix.getPosition3D() + velocity;

//        newBallPosition.z = (baryCordinates.x * closestTrianglePoint[0].z +
//                             baryCordinates.y * closestTrianglePoint[0].z +
//                             baryCordinates.z * closestTrianglePoint[0].z);

//        mPosition.setPosition(newBallPosition.x, newBallPosition.y, newBallPosition.z + ballZOffset);
//        //qDebug() << "ball is falling";
//    }

}

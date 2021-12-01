#include "plane.h"
#include "mariocube.h"
#include "triangle.h"
#include "cube.h"
#include "camera.h"
#include "sphere.h"
#include "light.h"
#include "factory.h"
#include "objreader.h"
#include "constants.h"
#include "skybox.h"
#include "player.h"
#include "lassurface.h"
#include "rollingball.h"
#include "contourlines.h"
#include "shape.h"
#include <QDebug>

#define EXISTS(x) storedMeshes.find(x) != storedMeshes.end()

Factory::Factory()
{
    for (uint32_t ID = 0; ID < gsl::MAX_OBJECTS; ++ID)
    {
        mAvailableIDs.push(ID);
    }
}

GameObject* Factory::createContourLines(GameObject* surfaceToContour)
{
    GameObject* objectToCreate;

    objectToCreate = new ContourLines(static_cast<LASsurface*>(surfaceToContour)->contourPoints);
    objectToCreate->getMeshComponent()->mDrawType = GL_LINES;
    objectToCreate->getMaterialComponent()->mShaderProgram = 1;
    objectToCreate->getMaterialComponent()->mTextureUnit = 0;
    objectToCreate->mObjectType = gsl::CONTOURLINES;

    objectToCreate->init();
    objectToCreate->ID = mAvailableIDs.front(); //Give ID to GameObject
    mAvailableIDs.pop();                        //Pop ID from queue of available IDs

    mGameObjects.push_back(objectToCreate);
    return objectToCreate;
}

GameObject* Factory::createObject(gsl::objectTypes type)
{

    GameObject* objectToCreate = nullptr;

    switch (type)
    {
    case gsl::CUBE:
        objectToCreate = new Cube;
        if(EXISTS("Cube")) //If cube mesh exists
        {
            objectToCreate->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Cube"]));
        }
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
        objectToCreate->getSphereCollisionComponent()->center = gsl::Vector3D( 0.0f,  0.0f,  0.0f);
        objectToCreate->getSphereCollisionComponent()->radius = 0.7;
        cubecounter++;
        objectToCreate->mObjectType = gsl::CUBE;
        objectToCreate->mObjectName = "Cube " + std::to_string(cubecounter);
        break;

    case gsl::PLANE:
        objectToCreate = new Plane;
        if(EXISTS("Plane")) //If Plane mesh exists
        {
            objectToCreate->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Plane"]));
        }
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
        planecounter++;
        objectToCreate->mObjectType = gsl::PLANE;
        objectToCreate->mObjectName = "Plane " + std::to_string(planecounter);
        break;

    case gsl::TRIANGLE:
        objectToCreate = new Triangle;
        if(EXISTS("Triangle")) //If Triangle mesh exists
        {
            objectToCreate->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Triangle"]));
        }
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
        trianglecounter++;
        objectToCreate->mObjectType = gsl::TRIANGLE;
        objectToCreate->mObjectName = "Triangle " + std::to_string(trianglecounter);
        break;

    case gsl::MARIOCUBE:
        objectToCreate = new MarioCube;
        if(EXISTS("MarioCube")) //If MarioCube mesh exists
        {
            objectToCreate->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["MarioCube"]));
        }
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
        objectToCreate->getSphereCollisionComponent()->center = gsl::Vector3D( 0.0f,  0.0f,  0.0f);
        objectToCreate->getSphereCollisionComponent()->radius = 0.5;
        mariocounter++;
        objectToCreate->mObjectType = gsl::MARIOCUBE;
        objectToCreate->mObjectName = "MarioCube " + std::to_string(mariocounter);
        break;

    case gsl::SPHERE:
        objectToCreate = new Sphere;
        if(EXISTS("Sphere")) //If Sphere mesh exists
        {
            objectToCreate->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Sphere"]));
        }
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 1;
        objectToCreate->getSphereCollisionComponent()->center = gsl::Vector3D( 0.0f,  0.0f,  0.0f);
        objectToCreate->getSphereCollisionComponent()->radius = 0.25;
        spherecounter++;
        objectToCreate->mObjectType = gsl::SPHERE;
        objectToCreate->mObjectName = "Sphere " + std::to_string(spherecounter);
        break;

    case gsl::CAMERA:
        objectToCreate = new Camera(45, 4/3);
        objectToCreate->getMaterialComponent()->mShaderProgram = 0;
        objectToCreate->getMaterialComponent()->mTextureUnit = 0;
        cameracounter++;
        objectToCreate->mObjectType = gsl::CAMERA;
        objectToCreate->mObjectName = "Camera " + std::to_string(cameracounter);
        break;

    case gsl::PLAYER:
        objectToCreate = new Player;
        if(EXISTS("Wario"))
        {
        objectToCreate->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Wario"]));
        }
        objectToCreate->getMaterialComponent()->mShaderProgram = 0;
        objectToCreate->getMaterialComponent()->mTextureUnit = 0;
        objectToCreate->mObjectType = gsl::PLAYER;
        objectToCreate->mObjectName = "Player";
        break;

    case gsl::SKYBOX:
        objectToCreate = new Skybox("../GEA2021/Assets/skybox.obj");
        objectToCreate->getMaterialComponent()->mShaderProgram = 3;
        objectToCreate->getMaterialComponent()->mTextureUnit = 3;

        //skyboxcounter++;
        objectToCreate->mObjectType = gsl::SKYBOX;
        objectToCreate->mObjectName = "Skybox";
        break;

    case gsl::LIGHT:
        objectToCreate = new Light;
        if(EXISTS("Light")) //If Light mesh exists
        {
        objectToCreate->setMeshComponent(static_cast<MeshComponent*>(storedMeshes["Light"]));
        }
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 0;
        lightCounter++;
        objectToCreate->mObjectType = gsl::LIGHT;
        objectToCreate->mObjectName = "Light " + std::to_string(lightCounter);
        break;

    case gsl::LASSURFACE:
        //objectToCreate = new LASsurface("../GEA2021/Assets/LAS/test_las.txt");
        objectToCreate = new LASsurface("../GEA2021/Assets/LAS/kongehuset.txt");
        objectToCreate->getMaterialComponent()->mShaderProgram = 1;
        objectToCreate->getMaterialComponent()->mTextureUnit = 2;
        objectToCreate->mObjectType = gsl::LASSURFACE;
        objectToCreate->mObjectName = "LASsurface";
        break;

    case gsl::ROLLINGBALL:
        objectToCreate = new Rollingball(3);
        objectToCreate->getMaterialComponent()->mShaderProgram = 0;
        objectToCreate->getMaterialComponent()->mTextureUnit = 0;
        objectToCreate->mObjectType = gsl::ROLLINGBALL;
        objectToCreate->mObjectName = "Rollingball" + std::to_string(ballCounter);
        ballCounter++;
        objectToCreate->getTransformComponent()->mMatrix.scale(0.25, 0.25, 0.25);
        break;

    default:
        break;

    }

    objectToCreate->init();
    objectToCreate->ID = mAvailableIDs.front(); //Give ID to GameObject
    mAvailableIDs.pop();                        //Pop ID from queue of available IDs

    mGameObjects.push_back(objectToCreate);
    return objectToCreate;
}

void Factory::saveMesh(std::string fileName, std::string nickName)
{
    if(EXISTS(nickName)){
        qDebug() << nickName.c_str() << "already exists.";
    }
    else {
        ObjReader objReader;
        MeshComponent* newMesh = new MeshComponent();
        //initMesh(newMesh);
        objReader.readFile(fileName, &newMesh->mVertices, &newMesh->mIndices);
        storedMeshes.insert(std::pair<std::string, MeshComponent*>{nickName, newMesh});
    }
}

void Factory::saveMesh(std::vector<Vertex> vertices, std::string nickName)
{
    if(EXISTS(nickName)){
        qDebug() << nickName.c_str() << "already exists.";
    }
    else {
        MeshComponent* newMesh = new MeshComponent();
        newMesh->mVertices = vertices;
        if(nickName == "TestCube")
        {
            initMesh(newMesh);
        }
        storedMeshes.insert(std::pair<std::string, MeshComponent*>{nickName, newMesh});
    }
}

void Factory::initMesh(MeshComponent *mesh)
{
    initializeOpenGLFunctions();

    glGenVertexArrays( 1, &mesh->mVAO );
    glBindVertexArray( mesh->mVAO );


    glGenBuffers( 1, &mesh->mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mesh->mVBO );

    glBufferData( GL_ARRAY_BUFFER,                     //what buffer type
                  mesh->mVertices.size() * sizeof( Vertex ), //how big buffer do we need
                  mesh->mVertices.data(),                    //the actual vertices
                  GL_STATIC_DRAW                       //should the buffer be updated on the GPU
                  );

    glBindBuffer(GL_ARRAY_BUFFER, mesh->mVBO);

    // 1rst attribute buffer : coordinates

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    if(mesh->mIndices.size() > 0)
    {
        //Second buffer - holds the indices (Element Array Buffer - EAB):
        glGenBuffers(1, &mesh->mEAB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->mEAB);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->mIndices.size() * sizeof(GLuint), mesh->mIndices.data(), GL_STATIC_DRAW);
    }

    glBindVertexArray(0);
}

void Factory::openLevel(Level level)
{
    std::multimap objects = level.objectsInLevel;

    for (auto it = objects.begin(); it != objects.end(); it++)
    {
        GameObject* temp = createObject(it->first);

        SpawnSettings settings = it->second;
        //Set initial location
        temp->move(settings.initialPos.getX(), settings.initialPos.getY(), settings.initialPos.getZ());
        //Set initial scale
        temp->getTransformComponent()->mMatrix.setScale(settings.initialScale.x, settings.initialScale.y, settings.initialScale.z);
        //Set initial rotation
        temp->getTransformComponent()->mMatrix.setRotation(settings.initialRot.x, settings.initialRot.y, settings.initialRot.z);
    }
}


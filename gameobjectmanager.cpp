#include "gameobjectmanager.h"

GameObjectManager::GameObjectManager()
{
    //This is a hack - to make sure my MeshComponens are not moved for now
    //Please fix
    //mMeshComponents.reserve(gsl::MAX_MESHCOMPONENTS);
}

GameObjectManager &GameObjectManager::getInstance()
{
    static GameObjectManager *mInstance = new GameObjectManager();
    return *mInstance;
}

//void GameObjectManager::manageObjects(/*mCurrentCamera*/)
//{
    //Axis
    //  GameObject *temp = new XYZ();
    //  temp->mMaterial->mShaderProgram = 0; //plain shader
    //  temp->init();
    //  mVisualObjects.push_back(temp);

    //int id;
    //string temp = "ost";
    //addObject(temp, id, 1, 1, 1, 0);

    //Dag's dog triangle
    //  temp = new Triangle();
    //  temp->init();
    //  temp->mMaterial->mShaderProgram = 1;    //texture shader
    //  temp->mMaterial->mTextureUnit = 1;      //dog texture
    //  temp->mTransform->mMatrix.translate(0.f, 0.f, .5f);
    //  mVisualObjects.push_back(temp);

    //Set up camera position
    //mCurrentCamera.setPosition(gsl::Vector3D(1.f, .5f, 4.f));
//}

GameObject *GameObjectManager::addObject(string name, int id, bool TransformID, int MeshID, int MaterialID, bool GravityID)
{
    GameObject* CurrentGameObject = new GameObject();

    CurrentGameObject->entitiyID = id;
    CurrentGameObject->mName = name;

    CurrentGameObject->init();

    if (TransformID == 1)
    {
        //new XYZ();
        CurrentGameObject->mTransform = new TransformComponent();
        CurrentGameObject->mTransform->mMatrix.translate(0.0f, 0.0f, 0.5f);
    }

    if (MeshID != 0)
    {
        CurrentGameObject->mMesh = new MeshComponent();

        if (MeshID == 1)        // 1 = dog texture
            {
                CurrentGameObject->mMesh = makeTriangle();
            }
    }

    if (MaterialID != 0)
    {
        CurrentGameObject->mMaterial = new MaterialComponent();
        //CurrentGameObject->mMaterial->mShaderProgram = MaterialID;
        CurrentGameObject->mMaterial->mTextureUnit = MaterialID;
    }

    if (GravityID != 0)
    {
        CurrentGameObject->mGravity = new GravityComponent();
        CurrentGameObject->mGravity->affectedByGravity = 1;
        CurrentGameObject->mGravity->gravitationalForce = 1^2;
    }

    id++;

    //GameObject->push_back(CurrentGameObject);

    return CurrentGameObject;
}

int GameObjectManager::makeAxis()
{
    //should check if this object is new before this!
    mMeshComponents.emplace_back(MeshComponent());
    MeshComponent &temp = mMeshComponents.back();

    temp.mVertices.push_back(Vertex{0.f, 0.f, 0.f, 1.f, 0.f, 0.f});
    temp.mVertices.push_back(Vertex{100.f, 0.f, 0.f, 1.f, 0.f, 0.f});
    temp.mVertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 1.f, 0.f});
    temp.mVertices.push_back(Vertex{0.f, 100.f, 0.f, 0.f, 1.f, 0.f});
    temp.mVertices.push_back(Vertex{0.f, 0.f, 0.f, 0.f, 0.f, 1.f});
    temp.mVertices.push_back(Vertex{0.f, 0.f, 100.f, 0.f, 0.f, 1.f});

    temp.mDrawType = GL_LINES;

    initMesh(temp);

    return mMeshComponents.size()-1;    //returns index to last object
}

int GameObjectManager::makeTriangle()
{
    //should check if this object is new before this!
    mMeshComponents.emplace_back(MeshComponent());
    MeshComponent &temp = mMeshComponents.back();
    temp.mVertices.push_back(Vertex{-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  0.f, 0.f}); // Bottom Left
    temp.mVertices.push_back(Vertex{0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.f}); // Bottom Right
    temp.mVertices.push_back(Vertex{0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 1.f}); // Top

    temp.mDrawType = GL_TRIANGLES;
    initMesh(temp);

    return mMeshComponents.size()-1;    //returns index to last object
}

void GameObjectManager::initMesh(MeshComponent &tempMeshComp)
{
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &tempMeshComp.mVAO );
    glBindVertexArray( tempMeshComp.mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &tempMeshComp.mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, tempMeshComp.mVBO );

    glBufferData( GL_ARRAY_BUFFER, tempMeshComp.mVertices.size()*sizeof(Vertex), tempMeshComp.mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : position
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)( 6 * sizeof( GLfloat ) ));
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    if(tempMeshComp.mIndices.size() > 0) {
        glGenBuffers(1, &tempMeshComp.mEAB);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tempMeshComp.mEAB);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, tempMeshComp.mIndices.size() * sizeof(GLuint), tempMeshComp.mIndices.data(), GL_STATIC_DRAW);
    }

    glBindVertexArray(0);

    tempMeshComp.mIndexCount = tempMeshComp.mIndices.size();
    tempMeshComp.mVertexCount = tempMeshComp.mVertices.size();
}

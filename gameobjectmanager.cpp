#include "gameobjectmanager.h"

void GameObjectManager::manageObjects(/*mCurrentCamera*/)
{

    //Axis
    //  GameObject *temp = new XYZ();
    //  temp->mMaterial->mShaderProgram = 0; //plain shader
    //  temp->init();
    //  mVisualObjects.push_back(temp);

    int id;
    string temp = "ost";
    addObject(temp, id, 1, 1, 1, 0);

    //Dag's dog triangle
    //  temp = new Triangle();
    //  temp->init();
    //  temp->mMaterial->mShaderProgram = 1;    //texture shader
    //  temp->mMaterial->mTextureUnit = 1;      //dog texture
    //  temp->mTransform->mMatrix.translate(0.f, 0.f, .5f);
    //  mVisualObjects.push_back(temp);

    //Set up camera position
    //mCurrentCamera.setPosition(gsl::Vector3D(1.f, .5f, 4.f));
}

void GameObjectManager::addObject(string name, int id, bool TransformID, int MeshID, int MaterialID, bool GravityID)
{
    GameObject * CurrentGameObject;

    CurrentGameObject->entitiyID = id;
    CurrentGameObject->mName = name;

    CurrentGameObject->init();

    if (TransformID == 1)
    {
        new XYZ();
        CurrentGameObject->mTransform->mMatrix.translate(0.0f, 0.0f, 0.5f);
    }

    if (MeshID != 0)
    {
        CurrentGameObject->mMaterial->mTextureUnit = MeshID;      //dog texture
    }

    if (MaterialID != 0)
    {
        CurrentGameObject->mMaterial->mShaderProgram = MaterialID;
        //CurrentGameObject->mMaterial->mTextureUnit = MasawdterialID;
    }

    if (GravityID != 0)
    {
        CurrentGameObject->mGravity->affectedByGravity = 1;
        CurrentGameObject->mGravity->gravitationalForce = 1^2;
    }

    //mGameObjects.push_back(CurrentGameObject);
    id++;
}

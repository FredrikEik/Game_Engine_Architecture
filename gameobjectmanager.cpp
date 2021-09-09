#include "gameobjectmanager.h"

void manageObjects()
{
    int id;
    //Axis
    //  GameObject *temp = new XYZ();
    //  temp->mMaterial->mShaderProgram = 0; //plain shader
    //  temp->init();
    //  mVisualObjects.push_back(temp);

    string temp = "ost";

        addObject(temp, id, 1, 1, 1, 0);

    //Dag's dog triangle
    //  temp = new Triangle();
    //  temp->init();
    //  temp->mMaterial->mShaderProgram = 1;    //texture shader
    //  temp->mMaterial->mTextureUnit = 1;      //dog texture
    //  temp->mTransform->mMatrix.translate(0.f, 0.f, .5f);
    //  mVisualObjects.push_back(temp);

    //GameObject *dagsDog;

    //Set up camera
    mCurrentCamera = new Camera();
    mCurrentCamera->setPosition(gsl::Vector3D(1.f, .5f, 4.f));
}

void addObject(std::string name, int id, int TransformID, int MeshID, int MaterialID, int GravityID)
{

    GameObject * CurrentGameObject;

    CurrentGameObject->entitiyID++;
    CurrentGameObject->mName = name;
    //GameObject *name;

    CurrentGameObject->init();

    if (TransformID == 1)
    {
        new XYZ();
        CurrentGameObject->mTransform->mMatrix.translate(0.0f, 0.0f, 0.5f);
    }

    if (MeshID != 0)
    {
        //Read from an obj file
    }

    if (MaterialID != 0)
    {
        CurrentGameObject->mMaterial->mShaderProgram = MaterialID;
        CurrentGameObject->mMaterial->mTextureUnit = MaterialID;
    }

    if (GravityID != 0)
    {
        //name->mGravity->mGravity = gravity;
    }
}

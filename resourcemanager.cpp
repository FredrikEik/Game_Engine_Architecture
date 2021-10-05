#include "resourcemanager.h"


ResourceManager::ResourceManager()
{
id = 0;
std::string name = "objName";


}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::manageObjects(std::string objectName, int ObjectID)
{

    //Axis
    //      VisualObject *temp = new XYZ();
    //      temp->mMaterial->mShaderProgram = 0; //plain shader
    //      temp->init();
    //      mVisualObjects.push_back(temp);

    name = objectName;
    addObject(name, id, ObjectID, 1, 1, 1);

    //Dag's dog triangle
//      temp = new Triangle();
//      temp->init();
//      temp->mMaterial->mShaderProgram = 1;    //texture shader
//      temp->mMaterial->mTextureUnit = 1;      //dog texture
//      temp->mTransform->mMatrix.translate(0.f, 0.f, .5f);
//      mVisualObjects.push_back(temp);

//    VisualObject *dagsDog;

    //Set up camera
//    Camera *mCurrentCamera;

//    mCurrentCamera = new Camera();
//    mCurrentCamera->setPosition(gsl::Vector3D(1.f, .5f, 4.f));

    id++;
}

void ResourceManager::addObject(std::string name, int id, int ObjectType,
                                int TransformID, int MeshID, int MaterialID)
{
     VisualObject * CurrentVisualObject = 0;

    if(ObjectType == 0) {
        //Axis
//              VisualObject *temp = new XYZ();
//              temp->mMaterial->mShaderProgram = 0; //plain shader
//              temp->init();
//              mVisualObjects.push_back(temp);
        CurrentVisualObject = new XYZ();
    }

    else if(ObjectType == 1) {
        //Triangle
        CurrentVisualObject = new Triangle();
    }

    else if(ObjectType == 2) {
        //Cube
        CurrentVisualObject = new Cube();
    }



    mVisualObjects.push_back(CurrentVisualObject);

    CurrentVisualObject->entitiyID = id;
    CurrentVisualObject->mName = name;

    CurrentVisualObject->init();

    if (TransformID == 1)
    {
        new XYZ();
        CurrentVisualObject->mTransform->mMatrix.translate(0.0f, 0.0f, 0.5f);
    }

    if (MeshID != 0)
    {
        //Read from an obj file
    }

    if (MaterialID != 0)
    {
        CurrentVisualObject->mMaterial->mShaderProgram = MaterialID;
        CurrentVisualObject->mMaterial->mTextureUnit = MaterialID;
    }

}

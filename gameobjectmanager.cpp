#include "gameobjectmanager.h"

void createObjects()
{
    //Axis
    VisualObject *temp = new XYZ();
    temp->mMaterial->mShaderProgram = 0; //plain shader
    temp->init();
    mVisualObjects.push_back(temp);

    //Dag's dog triangle
    temp = new Triangle();
    temp->init();
    temp->mMaterial->mShaderProgram = 1;    //texture shader
    temp->mMaterial->mTextureUnit = 1;      //dog texture
    temp->mTransform->mMatrix.translate(0.f, 0.f, .5f);
    mVisualObjects.push_back(temp);

    //Set up camera
    mCurrentCamera = new Camera();
    mCurrentCamera->setPosition(gsl::Vector3D(1.f, .5f, 4.f));
}

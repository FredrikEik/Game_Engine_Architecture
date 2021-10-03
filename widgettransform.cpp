#include "widgettransform.h"
#include "ui_widgettransform.h"
#include <QTreeWidget>
#include <QKeyEvent>

//#include "scene.h"
#include "gameobject.h"
#include "matrix4x4.h"
#include "rendersystem.h"
#include "components.h"

WidgetTransform::WidgetTransform(QWidget *parent) :
    QWidget(parent), ui(new Ui::WidgetTransform)
{
    ui->setupUi(this);
    setStepRates();
    setFocusPolicy(Qt::NoFocus);
}

WidgetTransform::~WidgetTransform()
{
    delete ui;
}

void WidgetTransform::readPosition()
{
    gsl::Vector3D position = mRenderSystem->mGameObjects[indexInSceneArray]->
            mTransform->mMatrix.getPosition();
    ui->doubleSpinBoxXPosition->setValue(position.x);
    ui->doubleSpinBoxYPosition->setValue(position.y);
    ui->doubleSpinBoxZPosition->setValue(position.z);
}

void WidgetTransform::readRotation()
{
    gsl::Vector3D rotation; // = mRenderSystem->mTransforms[indexInSceneArray].rotation;
    ui->doubleSpinBoxXRotation->setValue(rotation.x);
    ui->doubleSpinBoxYRotation->setValue(rotation.y);
    ui->doubleSpinBoxZRotation->setValue(rotation.z);
}

void WidgetTransform::readScale()
{
    gsl::Vector3D scale; // = mRenderSystem->mTransforms[indexInSceneArray].scale;
    ui->doubleSpinBoxXScale->setValue(scale.x);
    ui->doubleSpinBoxYScale->setValue(scale.y);
    ui->doubleSpinBoxZScale->setValue(scale.z);
}

void WidgetTransform::setPosition()
{
    gsl::Vector3D position;
    position.x = ui->doubleSpinBoxXPosition->value();
    position.y = ui->doubleSpinBoxYPosition->value();
    position.z = ui->doubleSpinBoxZPosition->value();
    mRenderSystem->mGameObjects[indexInSceneArray]->mTransform->mMatrix.setPosition(position);
}

void WidgetTransform::setStepRates()
{
    ui->doubleSpinBoxXPosition->setSingleStep(mPositionStep);
    ui->doubleSpinBoxYPosition->setSingleStep(mPositionStep);
    ui->doubleSpinBoxZPosition->setSingleStep(mPositionStep);
    ui->doubleSpinBoxXScale->setSingleStep(mScaleStep);
    ui->doubleSpinBoxYScale->setSingleStep(mScaleStep);
    ui->doubleSpinBoxZScale->setSingleStep(mScaleStep);
    ui->doubleSpinBoxXRotation->setSingleStep(mRotationStep);
    ui->doubleSpinBoxYRotation->setSingleStep(mRotationStep);
    ui->doubleSpinBoxZRotation->setSingleStep(mRotationStep);
}

void WidgetTransform::on_doubleSpinBoxXPosition_valueChanged(double arg1)
{
    setPosition();
//    mRenderSystem->mTransforms[indexInSceneArray].position.x = arg1;
//    mCurrentScene->mVisualObjects.at(indexInSceneArray)->mTransform.position.x = arg1;
//    mCurrentScene->mVisualObjects.at(indexInSceneArray)->updateMatrix();
}

void WidgetTransform::on_doubleSpinBoxYPosition_valueChanged(double arg1)
{
    setPosition();
//    mRenderSystem->mTransforms[indexInSceneArray].position.y = arg1;
//    mCurrentScene->mVisualObjects.at(indexInSceneArray)->mTransform.position.y = arg1;
//    mCurrentScene->mVisualObjects.at(indexInSceneArray)->updateMatrix();
}

void WidgetTransform::on_doubleSpinBoxZPosition_valueChanged(double arg1)
{
    setPosition();
//    mRenderSystem->mTransforms[indexInSceneArray].position.z = arg1;
//    mCurrentScene->mVisualObjects.at(indexInSceneArray)->mTransform.position.z = arg1;
//    mCurrentScene->mVisualObjects.at(indexInSceneArray)->updateMatrix();
}

void WidgetTransform::on_doubleSpinBoxXScale_valueChanged(double arg1)
{
//    mRenderSystem->mTransforms[indexInSceneArray].scale.x = arg1;
//    mCurrentScene->mVisualObjects.at(indexInSceneArray)->mTransform.scale.x = arg1;
//    mCurrentScene->mVisualObjects.at(indexInSceneArray)->updateMatrix();
}

void WidgetTransform::on_doubleSpinBoxYScale_valueChanged(double arg1)
{
//    mRenderSystem->mTransforms[indexInSceneArray].scale.y = arg1;
//    mCurrentScene->mVisualObjects.at(indexInSceneArray)->mTransform.scale.y = arg1;
//    mCurrentScene->mVisualObjects.at(indexInSceneArray)->updateMatrix();
}

void WidgetTransform::on_doubleSpinBoxZScale_valueChanged(double arg1)
{
//    mRenderSystem->mTransforms[indexInSceneArray].scale.z = arg1;
//    mCurrentScene->mVisualObjects.at(indexInSceneArray)->mTransform.scale.z = arg1;
//    mCurrentScene->mVisualObjects.at(indexInSceneArray)->updateMatrix();
}

void WidgetTransform::on_doubleSpinBoxXRotation_valueChanged(double arg1)
{
    if(arg1 <= -360.0 || arg1 >= 360.0)
    {
        arg1 = 0.0;
        ui->doubleSpinBoxXRotation->setValue(arg1);
    }
//    mRenderSystem->mTransforms[indexInSceneArray].rotation.x = arg1;
//    mCurrentScene->mVisualObjects.at(indexInSceneArray)->mTransform.rotation.x = arg1;
//    mCurrentScene->mVisualObjects.at(indexInSceneArray)->updateMatrix();
}

void WidgetTransform::on_doubleSpinBoxYRotation_valueChanged(double arg1)
{
    if(arg1 <= -360.0 || arg1 >= 360.0)
    {
        arg1 = 0.0;
        ui->doubleSpinBoxYRotation->setValue(arg1);
    }
//    mRenderSystem->mTransforms[indexInSceneArray].rotation.y = arg1;
//    mCurrentScene->mVisualObjects.at(indexInSceneArray)->mTransform.rotation.y = arg1;
//    mCurrentScene->mVisualObjects.at(indexInSceneArray)->updateMatrix();
}

void WidgetTransform::on_doubleSpinBoxZRotation_valueChanged(double arg1)
{
    if(arg1 <= -360.0 || arg1 >= 360.0)
    {
        arg1 = 0.0;
        ui->doubleSpinBoxZRotation->setValue(arg1);
    }
//    mRenderSystem->mTransforms[indexInSceneArray].rotation.z = arg1;
//    mCurrentScene->mVisualObjects.at(indexInSceneArray)->mTransform.rotation.z = arg1;
//    mCurrentScene->mVisualObjects.at(indexInSceneArray)->updateMatrix();
}

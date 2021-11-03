#include "detailswidget.h"
#include "ui_detailswidget.h"

#include "gameobject.h"
#include "matrix4x4.h"
#include "renderwindow.h"
#include "components.h"

DetailsWidget::DetailsWidget(QWidget *parent, float positionPace, float rotationPace, float scalePace) :
    QWidget(parent), ui(new Ui::DetailsWidget), mPositionPace{positionPace}, mRotationPace{rotationPace}, mScalePace{scalePace}
{
    ui->setupUi(this);
    setFocusPolicy(Qt::NoFocus);
    mfactory = new Factory();
}

DetailsWidget::~DetailsWidget()
{
    delete ui;
}

void DetailsWidget::init(Factory *factory, int index)
{
    mfactory = factory;
    inSceneArrayIndex = index;
    readPosition();
    readRotation();
    //readScale();
    clearFocus();
}

void DetailsWidget::readPosition()
{
    position = mfactory->mGameObjects[inSceneArrayIndex]->getTransformComponent()->mMatrix.getPosition();
    ui->DoubleSpinBoxXPosition->setValue(position.x);
    ui->DoubleSpinBoxYPosition->setValue(position.y);
    ui->DoubleSpinBoxZPosition->setValue(position.z);
}

void DetailsWidget::readRotation()
{
     position = mfactory->mGameObjects[inSceneArrayIndex]->getTransformComponent()->mMatrix.getRotation();
     ui->DoubleSpinBoxXRotation->setValue(rotation.x);
     ui->DoubleSpinBoxYRotation->setValue(rotation.y);
     ui->DoubleSpinBoxZRotation->setValue(rotation.z);
}

void DetailsWidget::readScale()
{
    // lagre en inverse av scale og rotation i matrix4x4, lag en set funksjon ved å først rotere/scale med inverse for å så bruke den nye verdien
    // til å rotere/scale
    scale = mfactory->mGameObjects[inSceneArrayIndex]->getTransformComponent()->mMatrix.getScale();
    ui->DoubleSpinBoxXScale->setValue(scale.x);
    ui->DoubleSpinBoxYScale->setValue(scale.y);
    ui->DoubleSpinBoxZScale->setValue(scale.z);
}

void DetailsWidget::setPosition()
{
        mfactory->mGameObjects[inSceneArrayIndex]->getTransformComponent()->mMatrix.setPosition(position.x,position.y,position.z);
        //mfactory->mGameObjects[inSceneArrayIndex]->getSphereCollisionComponent()->center = position;
}

void DetailsWidget::setRotation()
{
    mfactory->mGameObjects[inSceneArrayIndex]->getTransformComponent()->mMatrix.setRotation(rotation.x,rotation.y,rotation.z);
}

void DetailsWidget::setScale()
{
    mfactory->mGameObjects[inSceneArrayIndex]->getTransformComponent()->mMatrix.setScale(scale.x,scale.y,scale.z);
}

void DetailsWidget::on_DoubleSpinBoxXPosition_valueChanged(double arg1)
{
    position.x = ui->DoubleSpinBoxXPosition->value();
    setPosition();
}


void DetailsWidget::on_DoubleSpinBoxYPosition_valueChanged(double arg1)
{
    position.y = ui->DoubleSpinBoxYPosition->value();
    setPosition();
}


void DetailsWidget::on_DoubleSpinBoxZPosition_valueChanged(double arg1)
{
    position.z = ui->DoubleSpinBoxZPosition->value();
    setPosition();
}


void DetailsWidget::on_DoubleSpinBoxXRotation_valueChanged(double arg1)
{

    rotation.x = ui->DoubleSpinBoxXRotation->value();
    setRotation();
}


void DetailsWidget::on_DoubleSpinBoxYRotation_valueChanged(double arg1)
{
    //mfactory->mGameObjects[inSceneArrayIndex]->getTransformComponent()->mMatrix.rotateY(-rotation.y);
    rotation.y = ui->DoubleSpinBoxYRotation->value();
    setRotation();
}


void DetailsWidget::on_DoubleSpinBoxZRotation_valueChanged(double arg1)
{
    //mfactory->mGameObjects[inSceneArrayIndex]->getTransformComponent()->mMatrix.rotateZ(-rotation.z);
    rotation.z = ui->DoubleSpinBoxZRotation->value();
    setRotation();
}


void DetailsWidget::on_DoubleSpinBoxXScale_valueChanged(double arg1)
{
    scale.x = ui->DoubleSpinBoxXScale->value();
    setScale();
}


void DetailsWidget::on_DoubleSpinBoxYScale_valueChanged(double arg1)
{
    scale.y = ui->DoubleSpinBoxYScale->value();
    setScale();
}


void DetailsWidget::on_DoubleSpinBoxZScale_valueChanged(double arg1)
{
    scale.z = ui->DoubleSpinBoxZScale->value();
    setScale();
}




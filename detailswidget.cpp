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
    //readRotation();
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
    // rotation = mfactory->mGameObjects[inSceneArrayIndex]->getTransformComponent()->mMatrix.
}

void DetailsWidget::readScale()
{

}

void DetailsWidget::setPosition()
{
        mfactory->mGameObjects[inSceneArrayIndex]->getTransformComponent()->mMatrix.setPosition(position.x,position.y,position.z);
        mfactory->mGameObjects[inSceneArrayIndex]->getSphereCollisionComponent()->center = position;
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

}


void DetailsWidget::on_DoubleSpinBoxYRotation_valueChanged(double arg1)
{

}


void DetailsWidget::on_DoubleSpinBoxZRotation_valueChanged(double arg1)
{

}


void DetailsWidget::on_DoubleSpinBoxXScale_valueChanged(double arg1)
{

}


void DetailsWidget::on_DoubleSpinBoxYScale_valueChanged(double arg1)
{

}


void DetailsWidget::on_DoubleSpinBoxZScale_valueChanged(double arg1)
{

}




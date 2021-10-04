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
}

DetailsWidget::~DetailsWidget()
{
    delete ui;
}

void DetailsWidget::init(Renderwindow *renderWindow, int index)
{
    mRenderwindow = renderWindow;
    inSceneArrayIndex =index;
    readPosition();
    readRotation();
    readScale();
    clearFocus();
}

void DetailsWidget::readPosition()
{

}

void DetailsWidget::readRotation()
{

}

void DetailsWidget::readScale()
{

}

void DetailsWidget::setPosition()
{

}

void DetailsWidget::on_DoubleSpinBoxXPosition_valueChanged(double arg1)
{

}


void DetailsWidget::on_DoubleSpinBoxYPosition_valueChanged(double arg1)
{

}


void DetailsWidget::on_DoubleSpinBoxZPosition_valueChanged(double arg1)
{

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




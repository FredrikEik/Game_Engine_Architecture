#include "transformwidget.h"
#include "ui_transformwidget.h"
#include "rendersystem.h"
#include "coreengine.h"
#include "gameobject.h"

transformWidget::transformWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::transformWidget)
{

    ui->setupUi(this);


}

transformWidget::~transformWidget()
{
    delete ui;
}

void transformWidget::init(RenderSystem *rendersystem, int index)
{
    mRenderSystem = rendersystem;
    objectsInList = index;
}

void transformWidget::on_rotX_valueChanged(double arg1)
{

   ui->rotX->setValue(arg1);
   //tydligvis er y x i dette tilfelle..

   CoreEngine::getInstance()->boss->mTransform->mMatrix.rotateY(arg1);

}

void transformWidget::on_rotY_valueChanged(double arg1)
{
    ui->rotY->setValue(arg1);
    //mMainWindow->rotateobject(arg1);
    //mRenderSystem->rotateObj(arg1);
    mRenderSystem->mGameObjects[0]->mTransform->mMatrix.rotateZ(arg1);
}

void transformWidget::on_rotZ_valueChanged(double arg1)
{

}

//transform x
void transformWidget::on_scaleX_valueChanged(double arg1)
{
    ui->scaleX->setValue(arg1);
    CoreEngine::getInstance()->boss->mTransform->mMatrix.translateX(-arg1);
}
//transform y
void transformWidget::on_scaleY_valueChanged(double arg1)
{

}
//transform z
void transformWidget::on_scaleZ_valueChanged(double arg1)
{

}

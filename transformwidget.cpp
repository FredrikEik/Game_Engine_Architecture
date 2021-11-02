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

void transformWidget::init(MainWindow * mainwindow, RenderSystem *rendersystem, CoreEngine * coreengine)
{
    mRenderSystem = rendersystem;
    mCoreEngine = coreengine;
    mMainWindow = mainwindow;
}

void transformWidget::on_rotX_valueChanged(double arg1)
{
    //Helt klart en dårlig måte å gjøre dette på, men for mitt lille spill går det greit. prøver finne en bedre løsning

  // ui->rotX->setValue(arg1);

   if(mMainWindow->mCurrentSelectedItem->text() == mCoreEngine->player->objName)
       mCoreEngine->player->mTransform->mMatrix.rotateZ(arg1);
   if(mMainWindow->mCurrentSelectedItem->text() == mCoreEngine->enemy->objName)
       mCoreEngine->enemy->mTransform->mMatrix.rotateZ(arg1);
   if(mMainWindow->mCurrentSelectedItem->text() == mCoreEngine->boss->objName)
       mCoreEngine->boss->mTransform->mMatrix.rotateZ(arg1);
   if(mMainWindow->mCurrentSelectedItem->text() == mCoreEngine->axis->objName)
       mCoreEngine->axis->mTransform->mMatrix.rotateZ(arg1);

}

void transformWidget::on_rotY_valueChanged(double arg1)
{
   // ui->rotY->setValue(arg1);

    if(mMainWindow->mCurrentSelectedItem->text() == mCoreEngine->player->objName)
        mCoreEngine->player->mTransform->mMatrix.rotateY(arg1);
    if(mMainWindow->mCurrentSelectedItem->text() == mCoreEngine->enemy->objName)
        mCoreEngine->enemy->mTransform->mMatrix.rotateY(arg1);
    if(mMainWindow->mCurrentSelectedItem->text() == mCoreEngine->boss->objName)
        mCoreEngine->boss->mTransform->mMatrix.rotateY(arg1);
    if(mMainWindow->mCurrentSelectedItem->text() == mCoreEngine->axis->objName)
        mCoreEngine->axis->mTransform->mMatrix.rotateY(arg1);


}

void transformWidget::on_rotZ_valueChanged(double arg1)
{
   // ui->rotZ->setValue(arg1);
    if(mMainWindow->mCurrentSelectedItem->text() == mCoreEngine->player->objName)
        mCoreEngine->player->mTransform->mMatrix.rotateX(arg1);
    if(mMainWindow->mCurrentSelectedItem->text() == mCoreEngine->enemy->objName)
        mCoreEngine->enemy->mTransform->mMatrix.rotateX(arg1);
    if(mMainWindow->mCurrentSelectedItem->text() == mCoreEngine->boss->objName)
        mCoreEngine->boss->mTransform->mMatrix.rotateX(arg1);
    if(mMainWindow->mCurrentSelectedItem->text() == mCoreEngine->axis->objName)
        mCoreEngine->axis->mTransform->mMatrix.rotateX(arg1);
}

//transform x
void transformWidget::on_scaleX_valueChanged(double arg1)
{
    translatex = arg1;
    ui->scaleX->setValue(arg1);

    if(mMainWindow->mCurrentSelectedItem->text() == mCoreEngine->player->objName)
        mCoreEngine->player->mTransform->mMatrix.setPosition(translatex, translatey, translatez);
    if(mMainWindow->mCurrentSelectedItem->text() == mCoreEngine->enemy->objName)
        mCoreEngine->enemy->mTransform->mMatrix.setPosition(translatex, translatey, translatez);
    if(mMainWindow->mCurrentSelectedItem->text() == mCoreEngine->boss->objName)
        mCoreEngine->boss->mTransform->mMatrix.setPosition(translatex, translatey, translatez);
    if(mMainWindow->mCurrentSelectedItem->text() == mCoreEngine->axis->objName)
        mCoreEngine->axis->mTransform->mMatrix.setPosition(translatex, translatey, translatez);

}
//transform y
void transformWidget::on_scaleY_valueChanged(double arg1)
{
    translatey = arg1;
    ui->scaleY->setValue(arg1);
    if(mMainWindow->mCurrentSelectedItem->text() == mCoreEngine->player->objName)
        mCoreEngine->player->mTransform->mMatrix.setPosition(translatex, translatey, translatez);
    if(mMainWindow->mCurrentSelectedItem->text() == mCoreEngine->enemy->objName)
        mCoreEngine->enemy->mTransform->mMatrix.setPosition(translatex, translatey, translatez);
    if(mMainWindow->mCurrentSelectedItem->text() == mCoreEngine->boss->objName)
         mCoreEngine->boss->mTransform->mMatrix.setPosition(translatex, translatey, translatez);
    if(mMainWindow->mCurrentSelectedItem->text() == mCoreEngine->axis->objName)
        mCoreEngine->axis->mTransform->mMatrix.setPosition(translatex, translatey, translatez);
}
//transform z
void transformWidget::on_scaleZ_valueChanged(double arg1)
{
    translatez = arg1;
    ui->scaleZ->setValue(arg1);
    if(mMainWindow->mCurrentSelectedItem->text() == mCoreEngine->player->objName)
        mCoreEngine->player->mTransform->mMatrix.setPosition(translatex, translatey, translatez);
    if(mMainWindow->mCurrentSelectedItem->text() == mCoreEngine->enemy->objName)
        mCoreEngine->enemy->mTransform->mMatrix.setPosition(translatex, translatey, translatez);
    if(mMainWindow->mCurrentSelectedItem->text() == mCoreEngine->boss->objName)
         mCoreEngine->boss->mTransform->mMatrix.setPosition(translatex, translatey, translatez);
    if(mMainWindow->mCurrentSelectedItem->text() == mCoreEngine->axis->objName)
        mCoreEngine->axis->mTransform->mMatrix.setPosition(translatex, translatey, translatez);
}

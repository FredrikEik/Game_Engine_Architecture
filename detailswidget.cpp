#include "detailswidget.h"
#include "ui_detailswidget.h"
#include "system.h"
#include "gameobject.h"
#include "matrix4x4.h"
#include "rendersystem.h"
#include "components.h"
#include <QColorDialog>
#include <QColor>

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

void DetailsWidget::init(RenderSystem *systemIn, int index)
{
    renderSystem = systemIn;
    inSceneArrayIndex = index;
    readPosition();
    readRotation();
    readScale();
    readTextureAndShader();
    readLightStrengths();
    clearFocus();
}

void DetailsWidget::readPosition()
{
    position = renderSystem->gameObjects[inSceneArrayIndex]->getTransformComponent()->mMatrix.getPosition();
    ui->DoubleSpinBoxXPosition->setValue(position.x);
    ui->DoubleSpinBoxYPosition->setValue(position.y);
    ui->DoubleSpinBoxZPosition->setValue(position.z);
}

void DetailsWidget::readRotation()
{
     rotation = renderSystem->gameObjects[inSceneArrayIndex]->getTransformComponent()->mMatrix.getRotation();
     ui->DoubleSpinBoxXRotation->setValue(rotation.x);
     ui->DoubleSpinBoxYRotation->setValue(rotation.y);
     ui->DoubleSpinBoxZRotation->setValue(rotation.z);
}

void DetailsWidget::readScale()
{
    // lagre en inverse av scale og rotation i matrix4x4, lag en set funksjon ved å først rotere/scale med inverse for å så bruke den nye verdien
    // til å rotere/scale
    scale = renderSystem->gameObjects[inSceneArrayIndex]->getTransformComponent()->mMatrix.getScale();
    ui->DoubleSpinBoxXScale->setValue(scale.x);
    ui->DoubleSpinBoxYScale->setValue(scale.y);
    ui->DoubleSpinBoxZScale->setValue(scale.z);
}
void DetailsWidget::readTextureAndShader()
{
    int shaderIndex = renderSystem->gameObjects[inSceneArrayIndex]->getMaterialComponent()->mShaderProgram;
    int textureIndex = renderSystem->gameObjects[inSceneArrayIndex]->getMaterialComponent()->mTextureUnit;
    ui->comboBox->setCurrentIndex(shaderIndex);
    ui->comboBox_2->setCurrentIndex(textureIndex);
}
void DetailsWidget::readLightStrengths()
{
    if(renderSystem->gameObjects[inSceneArrayIndex]->mObjectType == gsl::LIGHT)
    {
        float ambientStrength =  dynamic_cast<Light*>(renderSystem->gameObjects[inSceneArrayIndex])->mAmbientStrength;
        float lightStrength =    dynamic_cast<Light*>(renderSystem->gameObjects[inSceneArrayIndex])->mLightStrength;
        float specularStrength = dynamic_cast<Light*>(renderSystem->gameObjects[inSceneArrayIndex])->mSpecularStrength;
        ui->horizontalSlider->setValue(ambientStrength*100);
        ui->horizontalSlider_2->setValue(lightStrength*100);
        ui->horizontalSlider_3->setValue(specularStrength*100);
    }
}
void DetailsWidget::setPosition()
{
    renderSystem->gameObjects[inSceneArrayIndex]->getTransformComponent()->mMatrix.setPosition(position.x,position.y,position.z);
    renderSystem->gameObjects[inSceneArrayIndex]->getSphereCollisionComponent()->center = position;
}

void DetailsWidget::setRotation()
{
    renderSystem->gameObjects[inSceneArrayIndex]->getTransformComponent()->mMatrix.setRotation(rotation.x,rotation.y,rotation.z);
}

void DetailsWidget::setScale()
{
    renderSystem->gameObjects[inSceneArrayIndex]->getTransformComponent()->mMatrix.setScale(scale.x,scale.y,scale.z);
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
    rotation.y = ui->DoubleSpinBoxYRotation->value();
    setRotation();
}


void DetailsWidget::on_DoubleSpinBoxZRotation_valueChanged(double arg1)
{ 
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




void DetailsWidget::on_comboBox_currentIndexChanged(int index)
{
    renderSystem->gameObjects[inSceneArrayIndex]->getMaterialComponent()->mShaderProgram = index;
}


void DetailsWidget::on_comboBox_2_currentIndexChanged(int index)
{
    renderSystem->gameObjects[inSceneArrayIndex]->getMaterialComponent()->mTextureUnit = index;
}


void DetailsWidget::on_pushButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose Color");
    if (color.isValid())
    {
        QRgb rgb = color.rgb();
        gsl::Vector3D vectorRGB = gsl::Vector3D(qRed(rgb), qGreen(rgb), qBlue(rgb));
        if(renderSystem->gameObjects[inSceneArrayIndex]->mObjectType == gsl::LIGHT)
        {
            dynamic_cast<Light*>(renderSystem->gameObjects[inSceneArrayIndex])->mAmbientColor = vectorRGB;
            //qDebug() << "tried to change ambient color";
        }
    }
}


void DetailsWidget::on_horizontalSlider_sliderMoved(int position)
{
    if(renderSystem->gameObjects[inSceneArrayIndex]->mObjectType == gsl::LIGHT)
    {
        float ambientStrength = float(position)/100;
        dynamic_cast<Light*>(renderSystem->gameObjects[inSceneArrayIndex])->mAmbientStrength = ambientStrength;
        //qDebug() << "tried to change ambient strength";
    }
}


void DetailsWidget::on_horizontalSlider_2_sliderMoved(int position)
{
    if(renderSystem->gameObjects[inSceneArrayIndex]->mObjectType == gsl::LIGHT)
    {
        float lightStrength = float(position)/100;
        dynamic_cast<Light*>(renderSystem->gameObjects[inSceneArrayIndex])->mLightStrength = lightStrength;
        //qDebug() << "tried to change light strength";
    }
}


void DetailsWidget::on_pushButton_2_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose Color");
    if (color.isValid())
    {
        QRgb rgb = color.rgb();
        gsl::Vector3D vectorRGB = gsl::Vector3D(qRed(rgb), qGreen(rgb), qBlue(rgb));
        if(renderSystem->gameObjects[inSceneArrayIndex]->mObjectType == gsl::LIGHT)
        {
            dynamic_cast<Light*>(renderSystem->gameObjects[inSceneArrayIndex])->mLightColor = vectorRGB;
            //qDebug() << "tried to change light color";
        }
    }
}


void DetailsWidget::on_horizontalSlider_3_sliderMoved(int position)
{
    if(renderSystem->gameObjects[inSceneArrayIndex]->mObjectType == gsl::LIGHT)
    {
        float specularStrength = float(position)/100;
        dynamic_cast<Light*>(renderSystem->gameObjects[inSceneArrayIndex])->mSpecularStrength = specularStrength;
        //qDebug() << "tried to change specular strength";
    }
}


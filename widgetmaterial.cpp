#include "widgetmaterial.h"
#include "ui_widgetmaterial.h"


//#include "scene.h"
#include "rendersystem.h"
//#include "visualobject.h"
#include "texturehandler.h"
#include "resourcemanager.h"
#include "shaderhandler.h"

WidgetMaterial::WidgetMaterial(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetMaterial)
{
    ui->setupUi(this);
}

WidgetMaterial::~WidgetMaterial()
{
    delete ui;
}

void WidgetMaterial::readMaterialData()
{
//    unsigned short tempMaterialID = mCurrentScene->mEntities[indexInSceneArray].mMaterialIDIndex;
//    qDebug() << mCurrentScene->mEntities[indexInSceneArray].mName.c_str();
//    mCurrentMaterial = ResourceManager::getInstance().mMaterial[tempMaterialID];
    auto it = ResourceManager::getInstance().mMaterialMap.begin();
    ui->materialName->setText(it->first.c_str());
//    qDebug() << "Shader" << mCurrentMaterial->mShader;
    fillShaders();
//    ui->shaderComboBox->setCurrentIndex(mCurrentMaterial->mShader);
    fillTextures();
//    fillTexturesDone = true;    //fillTextures triggers _currentIndexChanged signal
//    qDebug() << "GL Texture Id" << mCurrentMaterial->mGLTextureID;
//    unsigned short tempTextureIndex = ResourceManager::getInstance().findmTextureIndexByGLTextureID(mCurrentMaterial->mGLTextureID);

//    ui->textureComboBox->setCurrentIndex(tempTextureIndex);

//    QPalette pal;
//    QColor tempColor(mCurrentMaterial->mColor.x*255, mCurrentMaterial->mColor.y*255, mCurrentMaterial->mColor.z*255);
//    pal.setColor(QPalette::Window, tempColor);
//    ui->colorLabel->setPalette(pal);

//    ui->specularStrength->setValue(mCurrentMaterial->mSpecularStrenght);
//    ui->specularExponent->setValue(mCurrentMaterial->mSpecularExponent);
}

void WidgetMaterial::fillShaders()
{
    for (const auto &var : ResourceManager::getInstance().mShaders) {
        ui->shaderComboBox->addItem(var->mName.c_str());
    }
}

void WidgetMaterial::fillTextures()
{
    for (const auto &var: ResourceManager::getInstance().mTextureHandler->mTextureMap)
            ui->textureComboBox->addItem(var.first.c_str());
//    //TODO: This will break when GLTextureID is not the same as the texture placement in mTexture
//    ui->textureComboBox->setCurrentIndex(mCurrentMaterial->mGLTextureID);
}

void WidgetMaterial::on_specularStrength_valueChanged(double arg1)
{
//    mCurrentMaterial->mSpecularStrenght = arg1;
}

void WidgetMaterial::on_specularExponent_valueChanged(int arg1)
{
//    mCurrentMaterial->mSpecularExponent = arg1;
}

void WidgetMaterial::on_colorButton_clicked()
{
    QColor currentColor; //mCurrentMaterial->mColor.x*255, mCurrentMaterial->mColor.y*255, mCurrentMaterial->mColor.z*255);
    QColor newColor = QColorDialog::getColor(currentColor, this, "Select New Color");

    if (newColor.isValid()) {
        QPalette pal;
        pal.setColor(QPalette::Window, newColor);
        ui->colorLabel->setPalette(pal);
//        mCurrentMaterial->mColor.x = newColor.red()/255.f;  //maybe use QColor.redF instead?
//        mCurrentMaterial->mColor.y = newColor.green()/255.f;
//        mCurrentMaterial->mColor.z = newColor.blue()/255.f;
    }
}

void WidgetMaterial::on_textureComboBox_currentIndexChanged(int index)
{
//    if(fillTexturesDone && mCurrentMaterial->mGLTextureID != 0)
//    {
//        qDebug() << "Texture changed to:" << index;
//        mCurrentMaterial->mGLTextureID = ResourceManager::getInstance().mTexture[index]->mGLTextureID;
//    }
}

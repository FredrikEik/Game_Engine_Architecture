#include "widgetlight.h"
#include "ui_widgetlight.h"

//#include <QColorDialog>
//#include "components.h"
#include "rendersystem.h"

WidgetLight::WidgetLight(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetLight)
{
    ui->setupUi(this);
}

WidgetLight::~WidgetLight()
{
    delete ui;
}

void WidgetLight::readLightData(RenderSystem *renderSystem)
{
    mCurrentLight = renderSystem->mLight;
    fillTypes();    //just fills the dropdown - but only POINT is supported for now
    fillLightsDone = true;

    ui->lightStrength->setValue(mCurrentLight->mLightStrenght);
    ui->ambientStrength->setValue(mCurrentLight->mAmbientStrength);

    //Light color
    QPalette pal;
    QColor tempColor(mCurrentLight->mColor.x*255, mCurrentLight->mColor.y*255, mCurrentLight->mColor.z*255);
    pal.setColor(QPalette::Window, tempColor);
    ui->colorLabel->setPalette(pal);

    //Ambient color
    tempColor = QColor(mCurrentLight->mAmbientColor.x*255, mCurrentLight->mAmbientColor.y*255, mCurrentLight->mAmbientColor.z*255);
    pal.setColor(QPalette::Window, tempColor);
    ui->ambientColorLabel->setPalette(pal);
}

void WidgetLight::fillTypes()
{
    ui->lightType->addItem("Point");
    ui->lightType->addItem("Spot");
    ui->lightType->addItem("Directional");
    ui->lightType->setCurrentIndex(mCurrentLight->mType);
}

void WidgetLight::on_colorButton_clicked()
{
    QColor currentColor(mCurrentLight->mColor.x*255, mCurrentLight->mColor.y*255, mCurrentLight->mColor.z*255);
    QColor newColor = QColorDialog::getColor(currentColor, this, "Select New Color");

    if (newColor.isValid()) {
        QPalette pal;
        pal.setColor(QPalette::Window, newColor);
        ui->colorLabel->setPalette(pal);
        mCurrentLight->mColor.x = newColor.redF();
        mCurrentLight->mColor.y = newColor.greenF();
        mCurrentLight->mColor.z = newColor.blueF();
    }
}

void WidgetLight::on_lightStrength_valueChanged(double arg1)
{
    mCurrentLight->mLightStrenght = arg1;
}

void WidgetLight::on_lightType_currentIndexChanged(int index)
{
    if(fillLightsDone)
    {
        qDebug() << "Type changed to:" << index;
        mCurrentLight->mType = static_cast<gsl::LightTypes>(index);
    }
}

void WidgetLight::on_ambientColorButton_clicked()
{
    QColor currentColor(mCurrentLight->mAmbientColor.x*255, mCurrentLight->mAmbientColor.y*255, mCurrentLight->mAmbientColor.z*255);
    QColor newColor = QColorDialog::getColor(currentColor, this, "Select New Color");

    if (newColor.isValid()) {
        QPalette pal;
        pal.setColor(QPalette::Window, newColor);
        ui->ambientColorLabel->setPalette(pal);
        mCurrentLight->mAmbientColor.x = newColor.redF();
        mCurrentLight->mAmbientColor.y = newColor.greenF();
        mCurrentLight->mAmbientColor.z = newColor.blueF();
    }
}

void WidgetLight::on_ambientStrength_valueChanged(double arg1)
{
     mCurrentLight->mAmbientStrength = arg1;
}


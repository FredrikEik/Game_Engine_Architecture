#ifndef WIDGETLIGHT_H
#define WIDGETLIGHT_H

//#include <QWidget>
#include "components.h"

namespace Ui {
class WidgetLight;
}

class WidgetLight : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetLight(QWidget *parent = nullptr);
    ~WidgetLight();

    void readLightData(class RenderSystem* renderSystem);
    void fillTypes();

    struct LightComponet *mCurrentLight{nullptr};
    bool fillLightsDone{false};

private slots:
    void on_colorButton_clicked();

    void on_lightStrength_valueChanged(double arg1);

    void on_lightType_currentIndexChanged(int index);

    void on_ambientColorButton_clicked();

    void on_ambientStrength_valueChanged(double arg1);

    void on_specularExponent_valueChanged(int arg1);

    void on_specularStrenght_valueChanged(double arg1);

private:
    Ui::WidgetLight *ui;
};

#endif // WIDGETLIGHT_H

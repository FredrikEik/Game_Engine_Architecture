#ifndef WIDGETTRANSFORM_H
#define WIDGETTRANSFORM_H

#include <QWidget>

#include "vector3d.h"

namespace Ui {
    class WidgetTransform;
}

class WidgetTransform : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTransform(class QWidget *parent = nullptr);
    ~WidgetTransform();

    void readPosition();
    void readRotation();
    void readScale();
    void setPosition();

    void setStepRates();
    int indexInSceneArray{-1};
    class RenderSystem* mRenderSystem{nullptr};

    float mPositionStep{1.f};
    float mRotationStep{1.f};
    float mScaleStep{0.1f};

private slots:
    void on_doubleSpinBoxXPosition_valueChanged(double arg1);

    void on_doubleSpinBoxYPosition_valueChanged(double arg1);

    void on_doubleSpinBoxZPosition_valueChanged(double arg1);

    void on_doubleSpinBoxXScale_valueChanged(double arg1);

    void on_doubleSpinBoxYScale_valueChanged(double arg1);

    void on_doubleSpinBoxZScale_valueChanged(double arg1);

    void on_doubleSpinBoxXRotation_valueChanged(double arg1);

    void on_doubleSpinBoxYRotation_valueChanged(double arg1);

    void on_doubleSpinBoxZRotation_valueChanged(double arg1);

private:
    Ui::WidgetTransform *ui;

};

#endif // WIDGETTRANSFORM_H

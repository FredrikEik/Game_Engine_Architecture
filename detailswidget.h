#ifndef DETAILSWIDGET_H
#define DETAILSWIDGET_H

#include <QWidget>
#include "vector3d.h"


class Factory;

namespace Ui {
class DetailsWidget;
}

class DetailsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DetailsWidget(QWidget *parent = nullptr, float positionPace = 1.f, float rotationPace = 1.f, float scalePace = 1.f);
    ~DetailsWidget();

    void init(Factory *factory, int index);




private slots:
    void on_DoubleSpinBoxXPosition_valueChanged(double arg1);

    void on_DoubleSpinBoxYPosition_valueChanged(double arg1);

    void on_DoubleSpinBoxZPosition_valueChanged(double arg1);

    void on_DoubleSpinBoxXRotation_valueChanged(double arg1);

    void on_DoubleSpinBoxYRotation_valueChanged(double arg1);

    void on_DoubleSpinBoxZRotation_valueChanged(double arg1);

    void on_DoubleSpinBoxXScale_valueChanged(double arg1);

    void on_DoubleSpinBoxYScale_valueChanged(double arg1);

    void on_DoubleSpinBoxZScale_valueChanged(double arg1);

private:
    Ui::DetailsWidget *ui;

    void readPosition();
    void readRotation();
    void readScale();
    void setPosition();

    float mPositionPace{1.f};
    float mRotationPace{1.f};
    double mScalePace {0.1};

    int inSceneArrayIndex{-1};
    Factory *mfactory{nullptr};
    gsl::Vector3D position;
    gsl::Vector3D rotation = (0,0,0);
    gsl::Vector3D scale;

};

#endif // DETAILSWIDGET_H

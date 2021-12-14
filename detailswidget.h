#ifndef DETAILSWIDGET_H
#define DETAILSWIDGET_H

#include <QWidget>
#include "vector3d.h"


class System;

namespace Ui {
class DetailsWidget;
}

class DetailsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DetailsWidget(QWidget *parent = nullptr, float positionPace = 1.f, float rotationPace = 1.f, float scalePace = 1.f);
    ~DetailsWidget();

    void init(System *systemIn, int index);

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

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_2_currentIndexChanged(int index);

    void on_pushButton_clicked();

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_2_sliderMoved(int position);

    void on_pushButton_2_clicked();

    void on_horizontalSlider_3_sliderMoved(int position);

private:
    Ui::DetailsWidget *ui;

    void readPosition();
    void readRotation();
    void readScale();
    void readTextureAndShader();
    void readLightStrengths();
    void setPosition();
    void setRotation();
    void setScale();

    float mPositionPace{1.f};
    float mRotationPace{1.f};
    double mScalePace {0.1};

    int inSceneArrayIndex{-1};
    System* system;
    gsl::Vector3D position;
    gsl::Vector3D rotation;
    gsl::Vector3D scale;

};

#endif // DETAILSWIDGET_H

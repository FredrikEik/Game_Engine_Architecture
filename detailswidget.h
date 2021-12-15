#ifndef DETAILSWIDGET_H
#define DETAILSWIDGET_H

#include <QWidget>
#include "vector3d.h"

class RenderSystem;

///
///     \brief Class for holding and changing objects details
///     \author Jonas Bjørhei & Thobias G Halse Ovastrøm
///     \date 15/12/21
///
namespace Ui {
class DetailsWidget;
}

class DetailsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DetailsWidget(QWidget *parent = nullptr, float positionPace = 1.f, float rotationPace = 1.f, float scalePace = 1.f);
    ~DetailsWidget();
    ///Initiate the widget
    void init(RenderSystem *systemIn, int index);

private slots:
    ///Changes the value of the x position
    void on_DoubleSpinBoxXPosition_valueChanged(double arg1);
    ///Changes the value of the y position
    void on_DoubleSpinBoxYPosition_valueChanged(double arg1);
    ///Changes the value of the z position
    void on_DoubleSpinBoxZPosition_valueChanged(double arg1);
    ///Changes the value of the x rotation
    void on_DoubleSpinBoxXRotation_valueChanged(double arg1);
    ///Changes the value of the y rotation
    void on_DoubleSpinBoxYRotation_valueChanged(double arg1);
    ///Changes the value of the z rotation
    void on_DoubleSpinBoxZRotation_valueChanged(double arg1);
    ///Changes the value of the x scale
    void on_DoubleSpinBoxXScale_valueChanged(double arg1);
    ///Changes the value of the y scale
    void on_DoubleSpinBoxYScale_valueChanged(double arg1);
    ///Changes the value of the z scale
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
    /// Gets the position of the selected objects and reads in the values for the detail widget
    void readPosition();
    ///Gets the rotation of the selected objects and reads in the values for the detail widget
    void readRotation();
    ///Gets the scale of the selected objects and reads in the values for the detail widget
    void readScale();
    void readTextureAndShader();
    void readLightStrengths();
    ///Sets the position of the selected object
    void setPosition();
    ///Sets the rotation of the selected object
    void setRotation();
    ///Sets the scale of the selected object
    void setScale();
    ///The pace the position increase/decrease with each time the spinbox is clicked
    float mPositionPace{1.f};
    ///The pace the rotation increase/decrease with each time the spinbox is clicked
    float mRotationPace{1.f};
    ///The pace the scale increase/decrease with each time the spinbox is clicked
    double mScalePace {0.1};

    ///Index of the selected object
    int inSceneArrayIndex{-1};
    RenderSystem* renderSystem;
    ///object position
    gsl::Vector3D position;
    ///object rotation
    gsl::Vector3D rotation;
    ///object scale
    gsl::Vector3D scale;

};

#endif // DETAILSWIDGET_H

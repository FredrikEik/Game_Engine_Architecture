#ifndef WIDGETTRANSFORM_H
#define WIDGETTRANSFORM_H

class RenderSystem;

namespace Ui {
    class WidgetTransform;
}

class WidgetTransform : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTransform(QWidget *parent = nullptr, float positionStep =1.f,
                             float rotationStep = 1.f, float scaleStep = 1.f);
    ~WidgetTransform();

    void init(RenderSystem* renderSystem, int index);

    void setPositionStep(float newPositionStep);

    void setRotationStep(float newRotationStep);

    void setScaleStep(float newScaleStep);

private:
    void readPosition();
    void readRotation();
    void readScale();
    void setPosition();

    void setStepRates();
    Ui::WidgetTransform *ui;
    float mPositionStep{1.f};
    float mRotationStep{1.f};
    float mScaleStep{0.1f};

    int indexInSceneArray{-1};
    RenderSystem* mRenderSystem{nullptr};

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
};

#endif // WIDGETTRANSFORM_H

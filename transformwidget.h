#ifndef TRANSFORMWIDGET_H
#define TRANSFORMWIDGET_H

#include <QWidget>
#include "rendersystem.h"
namespace Ui {
class transformWidget;
}

class transformWidget : public QWidget
{
    Q_OBJECT

public:
    explicit transformWidget(QWidget *parent = nullptr);
    ~transformWidget();

private slots:
    void on_rotX_valueChanged(double arg1);

    void on_rotY_valueChanged(double arg1);

    void on_rotZ_valueChanged(double arg1);

    void on_scaleX_valueChanged(double arg1);

    void on_scaleY_valueChanged(double arg1);

    void on_scaleZ_valueChanged(double arg1);

private:
    Ui::transformWidget *ui;

    RenderSystem* mRenderSystem{nullptr};
};

#endif // TRANSFORMWIDGET_H

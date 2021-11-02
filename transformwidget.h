#ifndef TRANSFORMWIDGET_H
#define TRANSFORMWIDGET_H

#include <QWidget>

#include "mainwindow.h"
class RenderSystem;
class CoreEngine;
class MainWindow;
namespace Ui {
class transformWidget;

}

class transformWidget : public QWidget
{
    Q_OBJECT

public:
    explicit transformWidget(QWidget *parent = nullptr);
    ~transformWidget();

    void init(MainWindow * mainwindow, RenderSystem *rendersystem, CoreEngine* coreengine);



private slots:
    void on_rotX_valueChanged(double arg1);

    void on_rotY_valueChanged(double arg1);

    void on_rotZ_valueChanged(double arg1);

    void on_scaleX_valueChanged(double translatex);

    void on_scaleY_valueChanged(double arg1);

    void on_scaleZ_valueChanged(double arg1);

private:
    Ui::transformWidget *ui;
    MainWindow *mMainWindow{nullptr};

            //int objectsInList{-1};

    RenderSystem* mRenderSystem{nullptr};

    CoreEngine * mCoreEngine{nullptr};
    float translatex{0};
    float translatey{0};
    float translatez;
};

#endif // TRANSFORMWIDGET_H

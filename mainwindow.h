﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QMainWindow>
#include "shapefactory.h"


class QWidget;
class RenderWindow;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_pushButton_toggled(bool checked);

    void on_pushButton_2_toggled(bool checked);

    void on_actionAdd_Triangle_triggered();

    void on_actionAdd_Square_triggered();

    void on_actionAdd_Circle_triggered();

    void on_actionAdd_Monkey_triggered();

    void on_doubleSpinBoxX(double x,VisualObject *mVisualObject);
    void on_doubleSpinBoxY(double y, VisualObject *mVisualObject);
    void on_doubleSpinBoxZ(double z, VisualObject *mVisualObject);

private:
    void init();
    Ui::MainWindow *ui;


    QWidget *mRenderWindowContainer;
    ShapeFactory *mShapefactory;
    RenderWindow *mRenderWindow;

};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QMainWindow>
#include "shapefactory.h"
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItemIterator>

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

    void on_doubleSpinBoxX_valueChanged(double arg1);

    void on_doubleSpinBoxY_valueChanged(double arg1);

    void on_doubleSpinBoxZ_valueChanged(double arg1);

    void on_treeWidget_itemActivated(QTreeWidgetItem *item, int column);



    void on_treeWidget_viewportEntered();

  //  void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    void init();
    Ui::MainWindow *ui;


    QWidget *mRenderWindowContainer;
    ShapeFactory *mShapefactory;
    RenderWindow *mRenderWindow;
    VisualObject *mVisualObject;
    int ActiveObject = 0;

};

#endif // MAINWINDOW_H

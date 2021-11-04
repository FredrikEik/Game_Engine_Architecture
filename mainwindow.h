#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItemIterator>

class QWidget;
class RenderWindow;
class VisualObject;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void selectWithMousePick(int index);

private slots:
    void on_pushButton_toggled(bool checked);

    void on_pushButton_2_toggled(bool checked);

    void on_actionAdd_Triangle_triggered();

    void on_actionAdd_Circle_triggered();

    void on_actionAdd_Square_triggered();

    void on_actionAdd_Monkey_triggered();

    void on_treeWidget_viewportEntered();

    void on_doubleSpinBoxX_valueChanged(double arg1);

    void on_doubleSpinBoxY_valueChanged(double arg1);

    void on_doubleSpinBoxZ_valueChanged(double arg1);

    void on_treeWidget_itemActivated(QTreeWidgetItem *item, int column);

    void on_RotateX_clicked();

    void on_RotateY_clicked();

    void on_RotateZ_clicked();

    void on_ScalePlus_clicked();

    void on_ScaleMinus_clicked();

    void on_actionAdd_Big_Wall_triggered();

    void on_actionAdd_Small_Wall_triggered();

private:
    void init();
    Ui::MainWindow *ui;
    VisualObject *mVisualObject;
    int Objects = 0;
    QTreeWidgetItem *Widgetitem{nullptr};
    QTreeWidgetItem *mQTWItem{nullptr};

    QWidget *mRenderWindowContainer;
    RenderWindow *mRenderWindow;
};

#endif // MAINWINDOW_H

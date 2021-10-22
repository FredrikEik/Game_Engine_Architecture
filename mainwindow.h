#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void updateViewPort();

private slots:
    void on_pushButton_toggled(bool checked);

    void on_actionCustom_OBJ_triggered();

    void on_actionSuzanne_triggered();

    void on_actionCube_triggered();

    void on_actionSphere_triggered();

    void on_actionPlane_triggered();

    void on_pushButton_2_clicked();

    void on_treeWidget_activated(const QModelIndex &index);

    void on_treeWidget_viewportEntered();



    void on_treeWidget_itemActivated(class QTreeWidgetItem *item, int column);

    void on_PosX_valueChanged(double arg1);

    void on_PosY_valueChanged(double arg1);

    void on_PosZ_valueChanged(double arg1);



private:
    void init();
    Ui::MainWindow *ui;

    QWidget *mRenderWindowContainer;
    RenderWindow *mRenderWindow;

    int SelectedItem = 0;
};

#endif // MAINWINDOW_H

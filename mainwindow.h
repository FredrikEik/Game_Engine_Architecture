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
    void updateDetails();
    void setSelectedItem(int i);

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



    void on_pushButton_3_clicked();


    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_checkBox_toggled(bool checked);

    void on_actionControls_triggered();


private:
    void init();
    Ui::MainWindow *ui;

    QWidget *mRenderWindowContainer;
    RenderWindow *mRenderWindow;

    int SelectedItem = 0;
};

#endif // MAINWINDOW_H

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

private slots:
    void on_pushButton_toggled(bool checked);

    void on_actionCustom_OBJ_triggered();

    void on_actionSuzanne_triggered();

    void on_actionCube_triggered();

    void on_actionSphere_triggered();

    void on_actionPlane_triggered();

    void on_pushButton_2_clicked();

private:
    void init();
    Ui::MainWindow *ui;

    QWidget *mRenderWindowContainer;
    RenderWindow *mRenderWindow;
};

#endif // MAINWINDOW_H

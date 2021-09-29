#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QWidget;
class RenderWindow;
class GameEngine;

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

    void on_ToggleLOD_toggled(bool checked);

    void on_pb_toggleWireframe_toggled(bool checked);

    void on_ToggleFrustumCulling_toggled(bool checked);

    void on_PlayStop_toggled(bool checked);

    void on_pb_toggleCollisionBox_toggled(bool checked);

private:
    void init();
    Ui::MainWindow *ui;

    QWidget *mRenderWindowContainer{nullptr};
    RenderWindow *mRenderWindow{nullptr};
    GameEngine *mGameEngine{nullptr};


};

#endif // MAINWINDOW_H

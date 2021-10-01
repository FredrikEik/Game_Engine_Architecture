#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QWidget;
class RenderWindow;
class GameEngine;
class QListWidget;
class GameObject;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateList();

private slots:

    void on_ToggleLOD_toggled(bool checked);

    void on_pb_toggleWireframe_toggled(bool checked);

    void on_ToggleFrustumCulling_toggled(bool checked);

    void on_PlayStop_toggled(bool checked);

    void on_pb_toggleCollisionBox_toggled(bool checked);


    void on_listWidget_currentRowChanged(int currentRow);

    void on_TranslateXspinBox_valueChanged(double arg1);


    void on_TranslateYspinBox_valueChanged(double arg1);

    void on_TranslateZspinBox_valueChanged(double arg1);

    void on_RotateXspinBox_valueChanged(double arg1);

private:
    void init();
    Ui::MainWindow *ui;

    int ObjectListIndex;
    int lastIndex{0};
    int lastXrot{0};
    int xcounter{0};


    QWidget *mRenderWindowContainer{nullptr};
    RenderWindow *mRenderWindow{nullptr};
    GameEngine *mGameEngine{nullptr};

    QListWidget *listWidget{nullptr};

    std::vector<GameObject*> GameObjects{nullptr};


};

#endif // MAINWINDOW_H

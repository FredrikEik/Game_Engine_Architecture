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

    void initList();
    void updateList();
    void refreshList();
    void setID(int ID);
    void clean();

public slots:

    void on_PlayStop_toggled(bool checked);

private slots:

    void on_ToggleLOD_toggled(bool checked);

    void on_pb_toggleWireframe_toggled(bool checked);

    void on_ToggleFrustumCulling_toggled(bool checked);


    void on_pb_toggleCollisionBox_toggled(bool checked);


    void on_listWidget_currentRowChanged(int currentRow);

    void on_TranslateXspinBox_valueChanged(double arg1);


    void on_TranslateYspinBox_valueChanged(double arg1);

    void on_TranslateZspinBox_valueChanged(double arg1);

    void on_RotateXspinBox_valueChanged(double arg1);

    void on_actionCube_triggered();

    void on_actionPyramid_triggered();

    void on_actionSphere_triggered();

    void on_actionSuzanne_triggered();

    void on_actionCube_hovered();

    void on_ScaleXspinBox_valueChanged(double arg1);

    void on_actionDelete_Selected_triggered();

    void on_actionGetCurrentRow_triggered();

private:
    void init();
    Ui::MainWindow *ui;

    int ObjectListIndex{0};
    int lastIndex{0};
    int lastXrot{0};
    int xcounter{0};

    bool bCurrentlyDeleting = false;


    QWidget *mRenderWindowContainer{nullptr};
    RenderWindow *mRenderWindow{nullptr};
    GameEngine *mGameEngine{nullptr};

    QListWidget *listWidget{nullptr};

    std::vector<GameObject*> GameObjects{nullptr};


};

#endif // MAINWINDOW_H

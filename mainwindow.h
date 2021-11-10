#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QWidget;
class RenderWindow;
class GameEngine;
class QListWidget;
class QComboBox;
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

    void initComboboxTexture(std::vector<std::string> &textureNames);

    int getObjectListIndex() const;

public slots:

    void on_PlayStop_toggled(bool checked);

private slots:


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

    void on_saveScene_clicked();

    void on_loadScene_clicked();

    void on_actionLOD_toggle_toggled(bool arg1);

    void on_actionFrustum_Culling_toggle_toggled(bool arg1);

    void on_actionShow_Wireframe_toggle_toggled(bool arg1);

    void on_actionShow_All_Collision_Boxes_toggle_toggled(bool arg1);


    void on_comboBox_currentIndexChanged(int index);

    void on_addObject_released();

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
    QComboBox *comboBox{nullptr};

    std::vector<GameObject*> GameObjects{nullptr};
    std::vector<std::string> allTextures;
    int currentTextureIndex{0};


};

#endif // MAINWINDOW_H

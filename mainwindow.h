#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gameobject.h"

class QWidget;
class RenderSystem;
class QTreeWidgetItem;
class GameObject;
class System;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    ///Update outliner so newly created objects show up
    void updateOutliner(std::vector <GameObject *> &GameObjectData);
    ///Selects object by index
    void selectObjectByIndex(int indexIn);
	
    void init();

    QSurfaceFormat format;

public slots:
    void on_PlayPause_clicked(bool checked);



private slots:
    void on_pushButton_toggled(bool checked);

    void on_CreateObject_clicked();
    /// Makes it possible to click items in the outliner
    void on_outliner_itemClicked(QTreeWidgetItem *item, int column);

    void on_toggleFrustumCulling_clicked(bool checked);

    void on_SaveLevel_clicked();

    void on_LoadLevel_clicked();

    void on_createDropDownBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    class DetailsWidget* mDetailsWidget{nullptr};

    void clearLayout(QLayout *layout);

    QWidget *mRenderSystemContainer;
    RenderSystem *mRenderSystem;
    System* mSystem;
    QTreeWidgetItem *mCurrentEditItem{nullptr};
    QTreeWidgetItem* mSceneOutlinerRoot{nullptr};

    //GameObject *mGameObject;
    ///index for the selected object
    int mCurrentEditItemIndex{-1};  //index into selected GameObject

    float mPositionStep{1.f};
    float mRotationStep{1.f};
    float mScaleStep{0.1f};

    int itemToSpawn;
};

#endif // MAINWINDOW_H

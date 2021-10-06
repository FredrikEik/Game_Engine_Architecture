#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gameobject.h"

class QWidget;
class RenderWindow;
class QTreeWidgetItem;
class GameObject;
class Factory;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateOutliner(std::vector <GameObject *> &GameObjectData);

    void selectObjectByIndex(int indexIn);

private slots:
    void on_pushButton_toggled(bool checked);

    void on_createDropDownBox_currentTextChanged(const QString &arg1);

    void on_CreateObject_clicked();

    void on_outliner_itemClicked(QTreeWidgetItem *item, int column);

    void on_PlayPause_clicked(bool checked);

    void on_toggleFrustumCulling_clicked(bool checked);

private:
    void init();
    Ui::MainWindow *ui;
    class DetailsWidget* mDetailsWidget{nullptr};

    void clearLayout(QLayout *layout);

    QWidget *mRenderWindowContainer;
    RenderWindow *mRenderWindow;
    QTreeWidgetItem *mCurrentEditItem{nullptr};
    QTreeWidgetItem* mSceneOutlinerRoot{nullptr};

    //GameObject *mGameObject;

    int mCurrentEditItemIndex{-1};  //index into selected GameObject

    float mPositionStep{1.f};
    float mRotationStep{1.f};
    float mScaleStep{0.1f};

    std::string itemToSpawn;
};

#endif // MAINWINDOW_H

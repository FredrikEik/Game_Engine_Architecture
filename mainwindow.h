#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QWidget;
class RenderWindow;
class QTreeWidgetItem;
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


    void selectObjectByIndex(int indexIn);

private slots:
    void on_pushButton_toggled(bool checked);

    void on_createDropDownBox_currentTextChanged(const QString &arg1);

    void on_CreateObject_clicked();


    void on_twSceneOutliner_itemClicked(QTreeWidgetItem *item, int column);

private:
    void init();
    Ui::MainWindow *ui;

    void clearLayout(QLayout *layout);

    QWidget *mRenderWindowContainer;
    RenderWindow *mRenderWindow;
    QTreeWidgetItem *mCurrentEditItem{nullptr};
    QTreeWidgetItem* mSceneOutlinerRoot{nullptr};
    GameObject* mGameObject;

    int mCurrentEditItemIndex{-1};  //index into selected GameObject

    class DetailsWidget* mDetailsWidget{nullptr};

    std::string itemToSpawn;

    float mPositionStep{1.f};
    float mRotationStep{1.f};
    float mScaleStep{0.1f};
};

#endif // MAINWINDOW_H

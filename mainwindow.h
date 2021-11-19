#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QListWidgetItem"

//Forward declarations
class QWidget;
//class QListWidgetItem;
class QTreeWidgetItem;
class RenderSystem;
class CoreEngine;

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
    QListWidgetItem *mCurrentSelectedItem{nullptr};
    QTreeWidgetItem *mCurrentTreeItem{nullptr};


public slots:


    void on_pb_toggleWireframe_toggled(bool checked);
    void on_pb_togglePlay_toggled(bool checked);


private slots:
    void on_pushButton_clicked();


    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_actionAdd_Player_triggered();

    void on_actionAdd_Enemy_triggered();

    void on_actionAdd_XYZ_triggered();


    void on_pushButton_2_clicked();



private:
    void init();


    class transformWidget *mTransformWidget{nullptr};
    bool clicked = true;

    Ui::MainWindow *ui;
   // int currentIndex{-1};
    QString prevstring;
    bool itemFromSceneAdded = false;
    QWidget *mRenderWindowContainer{nullptr};
    RenderSystem *mRenderSystem{nullptr};
    CoreEngine *mCoreEngine{nullptr};
};

#endif // MAINWINDOW_H

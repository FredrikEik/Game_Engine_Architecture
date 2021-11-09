#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLFunctions_4_1_Core>



//Forward declarations
class QWidget;
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

    QWidget *mRenderWindowContrainer{nullptr};

    bool renderMousePick{false};

    void updateUI(const std::vector<class CoreEngine> &GameObjectData);

    void selectObjectByName(QString nameIn);

public slots:
    void on_actionAdd_Triangle_triggered();
    void on_actionAdd_Suzanne_triggered();
    void on_actionRender_MousePick_toggled(bool arg1);
    void on_treeWidgetSceneOutliner_itemClicked(class QTreeWidgetItem *item, int);

    void on_pb_toggleWireframe_toggled(bool checked);
    void on_pb_togglePlay_toggled(bool checked);

private:
    void init();
    Ui::MainWindow *ui;

    QWidget *mRenderWindowContainer{nullptr};
    RenderSystem *mRenderSystem{nullptr};
    CoreEngine *mCoreEngine{nullptr};
    QTreeWidgetItem *mCurrentEditItem{nullptr};
    int mCurrentEditItemIndex{-1};
    void clearLayout(class QLayout *layout);
};


#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class QWidget;
class RenderSystem;
class CoreEngine;
class GameObject;
class QListWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateHierarchy(const std::vector<GameObject *> &GameObjectData);

public slots:
    void on_actionAdd_Triangle_triggered();
    void on_actionAdd_Suzanne_triggered();

    void on_pb_toggleWireframe_toggled(bool checked);
    void on_pb_togglePlay_toggled(bool checked);

private:
    void init();
    Ui::MainWindow *ui;

    QWidget *mRenderWindowContainer{nullptr};
    RenderSystem *mRenderSystem{nullptr};
    CoreEngine *mCoreEngine{nullptr};
    QListWidget *mSceneHierarchyRoot{nullptr};
};

#endif // MAINWINDOW_H

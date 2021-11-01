#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GameObject.h"

class QWidget;
class RenderWindow;
class CoreEngine;
class ResourceManager;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    static MainWindow *getInstance();
    ~MainWindow();

    void makeObjList(std::vector<GameObject*> mGameObjects);


private slots:
    void on_pushButton_toggled(bool checked);
    void on_startGameButton_toggled(bool checked);
    void on_LODButton_toggled(bool checked);
    void on_FrustumToggle_toggled(bool checked);
    void on_CreateObjectButton_clicked();
    void on_ObjectDropdown_currentTextChanged(const QString &arg1);

private:
    void init();
    Ui::MainWindow *ui;

    QWidget *mRenderWindowContainer{nullptr};
    RenderWindow *mRenderWindow{nullptr};
    CoreEngine *mCoreEngine{nullptr};
    ResourceManager *mResourceManager{nullptr};

    std::string spawnObject{"suzanne.obj"};
};

#endif // MAINWINDOW_H

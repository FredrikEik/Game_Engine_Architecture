#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    ~MainWindow();


private slots:
    void on_pushButton_toggled(bool checked);
    void on_startGameButton_toggled(bool checked);
    void on_LODButton_toggled(bool checked);
    void on_CreateObjectButton_clicked();
    void on_objectDropdown_currentTextChanged(const QString &arg1);

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

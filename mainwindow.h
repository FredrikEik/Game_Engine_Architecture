#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

public slots:
    void on_actionAdd_Triangle_triggered();
    void on_actionAdd_Suzanne_triggered();

    void on_actionPlay_triggered(bool checked);

private slots:
    void on_actionToggle_backside_culling_toggled(bool checked);

    void on_actionFrustum_Culling_triggered(bool checked);

    void on_actionBasic_Editor_Controls_triggered();

    void on_actionWireframe_triggered(bool checked);

    void on_actionKudos_to_triggered();

    void on_actionAxis_triggered(bool checked);

    void on_actionGrid_triggered(bool checked);

private:
    void init();
    Ui::MainWindow *ui;

    QWidget *mRenderWindowContainer{nullptr};
    RenderSystem *mRenderSystem{nullptr};
    CoreEngine *mCoreEngine{nullptr};
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//Forward declarations
class QWidget;
class RenderWindow;
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

    void on_pb_toggleWireframe_toggled(bool checked);
    void on_pb_togglePlay_toggled(bool checked);

private:
    void init();
    Ui::MainWindow *ui;

    QWidget *mRenderWindowContainer{nullptr};
    RenderWindow *mRenderWindow{nullptr};
    CoreEngine *mCoreEngine{nullptr};
};

#endif // MAINWINDOW_H

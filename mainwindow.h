#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QWidget;
class RenderSystem;

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
    void on_ToggleWireframe_toggled(bool checked);

    void on_SpawnEntity_clicked();

    void on_SelectEntity_editTextChanged(const QString &arg1);

private:
    void init();
    Ui::MainWindow *ui;

    QWidget *mRenderWindowContainer;
    RenderSystem *mRenderSystem;
};

#endif // MAINWINDOW_H

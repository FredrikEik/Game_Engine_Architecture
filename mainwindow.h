#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QWidget;
class RenderWindow;

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

    void on_actionAdd_Triangle_triggered();

    void on_actionAdd_Suzanne_triggered();

private:
    void init();
    Ui::MainWindow *ui;

    QWidget *mRenderWindowContainer{nullptr};
    RenderWindow *mRenderWindow{nullptr};
};

#endif // MAINWINDOW_H

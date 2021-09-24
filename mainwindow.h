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

    void on_createDropDownBox_currentTextChanged(const QString &arg1);

    void on_CreateObject_clicked();

private:
    void init();
    Ui::MainWindow *ui;

    QWidget *mRenderWindowContainer;
    RenderWindow *mRenderWindow;

    std::string itemToSpawn;
};

#endif // MAINWINDOW_H

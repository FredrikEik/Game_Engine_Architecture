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

    void on_comboBox_activated(const QString &arg1);

    void on_pushButton_2_clicked();

    void on_comboBox_editTextChanged(const QString &arg1);

private:
    void init();
    Ui::MainWindow *ui;

    QWidget *mRenderWindowContainer;
    RenderWindow *mRenderWindow;

    std::string ObjectSpawn;
};

#endif // MAINWINDOW_H

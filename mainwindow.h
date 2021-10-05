#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndexList>


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

    void addObjectToWorldList(std::string name);
    void removeObjectFromWorldList();

    int getCurrentRow();

private slots:
    void on_pushButton_toggled(bool checked);

    void on_pushButton_2_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

//    void on_Value_x_valueChanged(double arg1);

    void on_PauseSound_clicked();

    void on_PlaySound_clicked();

    void on_StopSound_clicked();

    void on_WorldObjects_currentRowChanged(int currentRow);

    void on_deleteButton_clicked();

private:
    void init();
    Ui::MainWindow *ui;

    QWidget *mRenderWindowContainer;
    RenderWindow *mRenderWindow;

    std::string ObjectSpawn;
};

#endif // MAINWINDOW_H

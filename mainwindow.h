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

    void displayCurrentTransform(int index);

    int getWidth();
    int getHeight();

    void disableWorldObjects(bool disable);
    void setSelection(int index);

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

    void on_transformX_valueChanged(double arg1);
    void on_transformY_valueChanged(double arg1);
    void on_transformZ_valueChanged(double arg1);

    void on_rotationX_valueChanged(double arg1);
    void on_rotationY_valueChanged(double arg1);
    void on_rotationZ_valueChanged(double arg1);

    void on_scaleX_valueChanged(double arg1);
    void on_scaleY_valueChanged(double arg1);
    void on_scaleZ_valueChanged(double arg1);

    void on_startGame_clicked();

private:
    void init();
    Ui::MainWindow *ui;

    QWidget *mRenderWindowContainer;
    RenderWindow *mRenderWindow;

    std::string ObjectSpawn;
};

#endif // MAINWINDOW_H

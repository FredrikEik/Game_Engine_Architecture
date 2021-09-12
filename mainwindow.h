#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widget.h"
#include "systems/ecs/entity.h"

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

    void on_pushButton_2_clicked(bool checked);

    void on_comboBox_activated(int index);

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_doubleSpinBox_2_valueChanged(double arg1);

    void on_doubleSpinBox_3_valueChanged(double arg1);

private:
    void init();
    Ui::MainWindow *ui;
    std::string in;

    QWidget *mRenderWindowContainer;
    RenderWindow *mRenderWindow;
    Entity *mEntity;
};
void on_doubleSpinBox_valueChanged(double arg1);


#endif // MAINWINDOW_H

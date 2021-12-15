#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItemIterator>

class QWidget;
class RenderWindow;
class VisualObject;

/**
   I MainWindow har vi lagt til UI for editor-mode og play-mode.
   Vi bytter mellom modusene med en Play/Stop-knapp.
   Vi bruker dette for å se og endre objektdetaljer i editor-mode.
   Vi har poeng-teller og liv-teller i play-mode.
 */

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void selectWithMousePick(int index);
    void createStatusBar();
    void run();
    void LiveCount(int lives);
    void PointCount(int points);
private slots:
    void on_pushButton_toggled(bool checked);

    void on_pushButton_2_toggled(bool checked);

    void on_actionAdd_Circle_triggered();

    void on_actionAdd_Square_triggered();

    void on_actionAdd_Monkey_triggered();

    void on_treeWidget_viewportEntered();

    void on_doubleSpinBoxX_valueChanged(double arg1);

    void on_doubleSpinBoxY_valueChanged(double arg1);

    void on_doubleSpinBoxZ_valueChanged(double arg1);

    void on_treeWidget_itemActivated(QTreeWidgetItem *item, int column);

    void on_RotateX_clicked();

    void on_RotateY_clicked();

    void on_RotateZ_clicked();

    void on_ScalePlus_clicked();

    void on_ScaleMinus_clicked();
private:
    void init();
    Ui::MainWindow *ui;
    VisualObject *mVisualObject;
    int objectID = 0;
    QTreeWidgetItem *Widgetitem{nullptr};

    QWidget *mRenderWindowContainer;
    RenderWindow *mRenderWindow;
};

#endif // MAINWINDOW_H

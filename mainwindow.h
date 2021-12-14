#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QMainWindow>
#include "shapefactory.h"
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItemIterator>

/**
*MainWindow - Vi har nødvendige funksjoner for UI her.
*on_ToggleWF_toggled - Toggler WireFrame knapp.
*on_PlayButton_toggled - Toggler start og restart, gjemmer også editorMode UI i play mode.
*on_actionAdd_Square/circle/monkey_triggered - Knapp for å spawne objekter
*doubleSpinBoxX/Y/Z - Flytter på objektet med en spinbox
*on_treeWidget_itemActivated - Sjekker i bunn og grunn om objektet i treeWidgeten blir trykket på med mousepick eller på liste og setter ID utifra hvordan den blir trykket på.
*on_treeWidget_viewportEntered - Setter inn alle navn til objektene
*RotateX/Y/ZP - Knapp som roterer objektene med +5 grader
*RotateX/Y/ZM - Knapp som roterer objektene med -5 grader
*on_ScaleP/M - Skalerer objektene med enten 1.5 eller 0.5;.



 */

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
    void SelectWithMousePick(int index);
    void run();
    void LiveCount(int lives);
    void PointCount(int points);

private slots:
    void on_ToggleWF_toggled(bool checked);


    void on_PlayButton_toggled(bool checked);


    void on_actionAdd_Square_triggered();

    void on_actionAdd_Circle_triggered();

    void on_actionAdd_Monkey_triggered();


    void on_doubleSpinBoxX_valueChanged(double arg1);

    void on_doubleSpinBoxY_valueChanged(double arg1);

    void on_doubleSpinBoxZ_valueChanged(double arg1);


    void on_treeWidget_itemActivated(QTreeWidgetItem *item, int column);

    void on_treeWidget_viewportEntered();


    void on_RotateXP_clicked();

    void on_RotateYP_clicked();

    void on_RotateZP_clicked();


    void on_ScaleP_clicked();

    void on_ScaleM_clicked();


    void on_RotateYM_clicked();

    void on_RotateZM_clicked();

    void on_RoatetXM_clicked();





private:
    void init();
    Ui::MainWindow *ui;
    QWidget *mRenderWindowContainer;
    QTreeWidgetItem *Widgetitem{nullptr};
    ShapeFactory *mShapefactory;
    RenderWindow *mRenderWindow;
    VisualObject *mVisualObject;
    int ActiveObject = 0;

};

#endif // MAINWINDOW_H

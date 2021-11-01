#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//Forward declarations
class QWidget;
class QListWidgetItem;
class RenderSystem;
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
    void on_actionAdd_Goat_triggered();

    void on_pb_toggleWireframe_toggled(bool checked);
    void on_pb_togglePlay_toggled(bool checked);

    void on_actionRotate_Transform_triggered();


private slots:
    void on_pushButton_clicked();


    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    void init();
    Ui::MainWindow *ui;

    class transformWidget *mTransformWidget{nullptr};
    bool clicked = true;

    QString prevstring;
    QListWidgetItem *mCurrentEditItem{nullptr};
    QWidget *mRenderWindowContainer{nullptr};
    RenderSystem *mRenderSystem{nullptr};
    CoreEngine *mCoreEngine{nullptr};
};

#endif // MAINWINDOW_H

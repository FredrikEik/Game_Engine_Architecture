#include "mainwindow.h"
#include <QApplication>
#include <QLabel>
#include <QWidget>
#include <QtWidgets>

int main(int argc, char *argv[])
{
    //Forces the usage of desktop OpenGL
    //Attribute must be set before Q(Gui)Application is constructed:
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

    //Makes an Qt application
    QApplication a(argc, argv);

//    widget myWidget;
//    myWidget.show();


    //Makes the Qt MainWindow and shows it.
    MainWindow w;
    w.show();

    return a.exec();
}

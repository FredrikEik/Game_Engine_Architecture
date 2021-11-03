#include "mainwindow.h"
#include <QPixmap>
#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    //Forces the usage of desktop OpenGL
    //Attribute must be set before Q(Gui)Application is constructed:
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

    //Makes an Qt application
    QApplication a(argc, argv);
    QPixmap pixmap(":/Assets/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage("Loading");
    QCoreApplication::processEvents();


    //Makes the Qt MainWindow and shows it.
    MainWindow w;

    w.move(10, 10);     //Moves the program at start. Nice if you have large screen and editor on one side.
    w.show();
    //splash.hide();
    splash.finish(&w);

    return a.exec();
}


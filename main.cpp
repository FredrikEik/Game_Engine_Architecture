#include "mainwindow.h"
#include <QPixmap>
#include <QApplication>
<<<<<<< HEAD
<<<<<<< HEAD
#include <Qt>
#include <QSplashScreen>
=======
>>>>>>> parent of 33b8247 (test)
=======
>>>>>>> parent of 33b8247 (test)

int main(int argc, char *argv[])
{
    //Forces the usage of desktop OpenGL
    //Attribute must be set before Q(Gui)Application is constructed:
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

    //Makes an Qt application
    QApplication a(argc, argv);
<<<<<<< HEAD
<<<<<<< HEAD
    QPixmap pixmap("../GEA2021/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage("Loading...");
    splash.raise();
    QCoreApplication::processEvents();
=======
>>>>>>> parent of 33b8247 (test)
=======
>>>>>>> parent of 33b8247 (test)

    //Makes the Qt MainWindow and shows it.
    MainWindow w;

    w.move(10, 10);     //Moves the program at start. Nice if you have large screen and editor on one side.
    w.show();
    splash.raise();
    //splash.hide();
    splash.finish(&w);

    return a.exec();
}


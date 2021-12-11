#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    /** Testing Doxygen Compile, This is in main.cpp
        Here some OpenGL, the QApplication, the splash-screen and the QT window gets made.
     */
    //Forces the usage of desktop OpenGL
    //Attribute must be set before Q(Gui)Application is constructed:
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

    //Makes an Qt application
    QApplication a(argc, argv);

    QSplashScreen *mSplash = new QSplashScreen;
    mSplash->setPixmap(QPixmap(":/AppGraphics/AppIcons/Gautes-splash.png")); // splash picture
    mSplash->show();

    //Makes the Qt MainWindow and shows it.
    MainWindow w;
    w.move(10, 10);     //Moves the program at start. Nice if you have large screen and editor on one side.
    w.show();
    mSplash->hide();

    return a.exec();
}

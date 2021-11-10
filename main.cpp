#include "mainwindow.h"
#include <QPixmap>
#include <QApplication>
#include <QSplashScreen>
#include "level.h"
#include <QApplication>
#include <QDebug>       //Using qDebug
#include <QFile>        //Reading from file
#include <QJSEngine>

int main(int argc, char *argv[])
{
    //Forces the usage of desktop OpenGL
    //Attribute must be set before Q(Gui)Application is constructed:
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

    //Makes an Qt application
    QApplication a(argc, argv);
    QPixmap pixmap("../GEA2021/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();

    splash.raise();
    splash.showMessage("Loading...");
    QCoreApplication::processEvents();

    QJSEngine jsEngine;

    //Makes the Qt MainWindow and shows it.
    MainWindow w;

    w.move(10, 10);     //Moves the program at start. Nice if you have large screen and editor on one side.
    w.show();
    splash.raise();

    //splash.hide();
    splash.finish(&w);

    /*
    QString fileName = "../GEA2021/Scripts/levelscript.js";
    QFile scriptFile(fileName);
    if (!scriptFile.open(QIODevice::ReadOnly))
        qDebug() << "Error - File does not exist: " << fileName;

    QTextStream stream (&scriptFile);
    QString contents = stream.readAll();

    scriptFile.close();

    jsEngine.evaluate(contents, fileName);

    Level *level = new Level;
    QJSValue objectTest = jsEngine.newQObject(level);
    jsEngine.globalObject().setProperty("cObject", objectTest);

    QJSValue spawnAllFunc = jsEngine.evaluate("spawnObjectsFromLevel");
    spawnAllFunc.call();
    */

    return a.exec();
}


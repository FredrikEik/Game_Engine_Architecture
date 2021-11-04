#include "mainwindow.h"
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

    QJSEngine jsEngine;

    //Makes the Qt MainWindow and shows it.
    MainWindow w;
    w.show();

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


    return a.exec();
}

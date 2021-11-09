#include "scriptsystem.h"
#include "gameobject.h"
#include "components.h"
#include "resourcemanager.h"
#include <QFile>
#include <QtDebug>
#include <string>

scriptsystem::scriptsystem()
{

}
void scriptsystem::readData(std::string scriptName)
{
    std::string fileName = gsl::ScriptFilePath + scriptName;
    QFile scriptFile(fileName.c_str());

    if (!scriptFile.open(QIODevice::ReadOnly))
        qDebug() << "Error - no file: " << fileName.c_str();

    QTextStream stream(&scriptFile);
    QString contents = stream.readAll();
    scriptFile.close();

    mScriptEngine.contains(contents);

    qDebug() << "Player";
    QJsonValue tempVar = mScriptEngine.contains("Name");
    qDebug() << "Name: " << tempVar.toString();
    tempVar = mScriptEngine.contains("Health");
    qDebug() << "Health: " << tempVar.toDouble() << "%";
    tempVar = mScriptEngine.contains("Speed");
    qDebug() << "Speed: " << tempVar.toDouble();
}

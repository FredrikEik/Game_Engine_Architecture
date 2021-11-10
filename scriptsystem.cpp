#include "scriptsystem.h"
#include "gameobject.h"
#include "components.h"
#include "resourcemanager.h"
#include <QFile>
#include <QtDebug>
#include <string>

scriptsystem::scriptsystem(QObject *parent) : QObject(parent)
{

}
void scriptsystem::scriptFunction(float in)
{
    qDebug()  << "kake" << in << "\n";
}
float scriptsystem::getSpeed() const
{
    return mSpeed;
}
void scriptsystem::setSpeed(float value)
{
    mSpeed = value;
}
void scriptsystem::sendSignal()
{
    emit signalOne();
}
int scriptsystem::commonFunc()
{
    int a{234};
    return a;
}



//void scriptsystem::readData(std::string scriptName)
//{
//    std::string fileName = gsl::ScriptFilePath + scriptName;
//    QFile scriptFile(fileName.c_str());

//    QTextStream stream(&scriptFile);
//    QString contents = stream.readAll();
//    scriptFile.close();

//    mScriptEngine.evaluate(contents, fileName.c_str());

//    qDebug() << "Player";
////    QJSValue tempVar = mScriptEngine.evaluate("Name");
//    QJsonValue tempVar = mScriptEngine.contains("Name");
//    qDebug() << "Name: " << tempVar.toString();
//    tempVar = mScriptEngine.contains("Health");
//    qDebug() << "Health: " << tempVar.toDouble() << "%";
//    tempVar = mScriptEngine.contains("Speed");
//    qDebug() << "Speed: " << tempVar.toDouble();
//}
//void scriptsystem::setData(unsigned short entityID, std::string scriptName)
//{
//    std::string fileName = gsl::ScriptFilePath + scriptName;
//    QFile scriptFile(fileName.c_str());

//    QTextStream stream(&scriptFile);
//    QString contents = stream.readAll();
//    scriptFile.close();

//    mScriptEngine.evaluate(contents, fileName.c_str());

////    QJSValue tempVar = mScriptEngine.evaluate("Name");
//    QJsonValue tempVar = mScriptEngine.evaluate("Name");
//    ResourceManager::getInstance().addObject(entityID)->m
//}




































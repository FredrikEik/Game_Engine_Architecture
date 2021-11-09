#include "scriptmanager.h"
#include "components.h"
#include <QString>
#include <QFile>

ScriptManager::ScriptManager(QJSEngine *engineRef, GameObject* object)
{
    gameObject = object;
    engine = engineRef;
}

void ScriptManager::initScript()
{
    JavaScriptComponent* scriptComp = gameObject->getScriptComponent();
    QString fileName = QString::fromStdString(scriptComp->scriptToRun);
    //Make a QFile for it
    QFile scriptFile(fileName);

    //Try to open file and give error if something is wrong
    if (!scriptFile.open(QIODevice::ReadOnly))
        qDebug() << "Error - NO FILE HERE: " << fileName;

    //reads the file
    QTextStream stream(&scriptFile);
    QString contents = stream.readAll();
    //now "contents" holds the whole JavaScript

    //close the file, because we don't need it anymore
    scriptFile.close();

    //Loads the whole script into script engine:
    //The important part! fileName is used to report bugs in the file
    engine->evaluate(contents, fileName);

    QJSValue refToSelf = engine->newQObject(this);
//    //Make a name for the object in the script engine
    engine->globalObject().setProperty("cObject", refToSelf);

    tickJSFunc = engine->evaluate("tick");
    runJSFunc = engine->evaluate("run");
}
void ScriptManager::runScript()
{
    runJSFunc.call();
}

void ScriptManager::tickScript()
{
    tickJSFunc.call();
}
void ScriptManager::moveObject(float x, float y, float z)
{
    gameObject->move(x,y,z);
}

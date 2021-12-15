#include "scriptsystem.h"
#include "gameobject.h"
#include "components.h"
#include "resourcemanager.h"
#include <QFile>
#include <QtDebug>
#include <string>
#include "scene.h"
#include "coreengine.h"

/********************************************//**
* ... making script system to take inn data and using it for the entities in the game
* and giving value to the different variables used in the game.
* It is a simple scripting system that can be further developed
* I have done most of the work on this for the engine with inspiration that can be seen in
* sources on the added doc
***********************************************/

ScriptSystem* ScriptSystem::mInstance = nullptr;    //static pointer to instance

ScriptSystem::ScriptSystem() : mDevice{nullptr}
{}

bool ScriptSystem::init()
{
    qDebug() << "Intializing OpenAL!";
    mDevice = alcOpenDevice(NULL);
    if (mDevice)
    {
        mContext = alcCreateContext(mDevice, NULL);
        alcMakeContextCurrent(mContext);
    }

    // Generate buffers
    alGetError();

    if (!mDevice)
    {
        qDebug() << "Device not made!";
    }
    else
        qDebug() << "OpenAL intialization complete!";


    return true;
}

void ScriptSystem::readData(std::string scriptN)
{
    std::string fileName = gsl::ScriptFilePath + scriptN;
    QFile scriptFile(fileName.c_str());

    QTextStream stream(&scriptFile);
    QString content = stream.readAll();
    scriptFile.close();

    mScriptEngine.evaluate(content, fileName.c_str());
    QJSValue tempVariable = mScriptEngine.evaluate("Name");
    tempVariable = mScriptEngine.evaluate("Health");
    tempVariable = mScriptEngine.evaluate("Speed");
    tempVariable = mScriptEngine.evaluate("Strength");

}
void ScriptSystem::setData(unsigned short entityID, std::string scriptN)
{
    std::string fileName = gsl::ScriptFilePath + scriptN;
    QFile scriptFile(fileName.c_str());

    QTextStream stream(&scriptFile);
    QString content = stream.readAll();
    scriptFile.close();


    mScriptEngine.evaluate(content, fileName.c_str());
    QJSValue tempVariable = mScriptEngine.evaluate("Name");
    ResourceManager::getInstance().mCoreEngine->mEntity[entityID].mScript.pName = tempVariable.toString();
    tempVariable = mScriptEngine.evaluate("Health");
    ResourceManager::getInstance().mCoreEngine->mEntity[entityID].mScript.pHp = tempVariable.toInt();
    tempVariable = mScriptEngine.evaluate("Speed");
    ResourceManager::getInstance().mCoreEngine->mEntity[entityID].mScript.pSpeed = tempVariable.toNumber();
    tempVariable = mScriptEngine.evaluate("Strength");
    ResourceManager::getInstance().mCoreEngine->mEntity[entityID].mScript.pStrength = tempVariable.toNumber();

}





































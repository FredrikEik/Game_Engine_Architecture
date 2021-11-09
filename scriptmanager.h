#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include "gameobject.h"
#include <QObject>
#include <QJSEngine>

class ScriptManager : public QObject
{
    Q_OBJECT

public:
    ScriptManager(QJSEngine *engineRef, GameObject *gameObject);

    void initScript();
    void runScript();
    void tickScript();

    Q_INVOKABLE void moveObject(float x, float y, float z);

    GameObject* gameObject;
    QJSEngine* engine;
    QJSValue tickJSFunc;
    QJSValue runJSFunc;
};

#endif // SCRIPTMANAGER_H

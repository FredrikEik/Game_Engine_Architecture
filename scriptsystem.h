#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H


#include "constants.h"
#include <string>
#include <qjsonvalue.h>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
//#include <QJSEngine>

#include <QObject>


class scriptsystem : public QObject
{
    Q_OBJECT

    Q_PROPERTY(float speed READ getSpeed WRITE setSpeed)
public:
    scriptsystem(QObject *parent = nullptr);

    void sendSignal();

    Q_INVOKABLE int commonFunc();

    //Setter og getter for speed-variabelen
    //navnene er de samme som i Q_PROPERTY over
    float getSpeed() const;
    void setSpeed(float value);

signals:
    void signalOne();

public slots:
//  en public slot kan kalles fra javascript, uten Q_INVOKABLE
    void scriptFunction(float in);

private:
//  denne kan ikke kalles, fordi den er private
//    Q_INVOKABLE void privateFunc();

//  denne har setters og getters som gjennom Q_PROPERTY kan brukes fra
//  javascript - der den heter bare "speed" som angitt i Q_PROPERTY
    float mSpeed{4.234f};

//    void readData(std::string scriptName);

//    void setData(MAX_ENTITIES_TYPE entityID, std::string scriptName);

//    QJSEngine mScriptEngine;


};

#endif // SCRIPTSYSTEM_H

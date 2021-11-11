#ifndef SCRIPT_H
#define SCRIPT_H

#include <QObject>


class Script : public QObject
{
public:
    Script(QObject *parent = nullptr);

    //  for å kalle funksjoner fra javascript
        Q_OBJECT

    //  For å bruke en C++ variabel i javascript må du bruke Q_PROPERTY
    //  Dette er navn og funksjonskall du kan bruke fra javascript
    //           variabel  navn     lesfunk       skrivfunc        signal ved forandring
        Q_PROPERTY(float speed READ getSpeed WRITE setSpeed)    // NOTIFY valueChanged)
                                                                //get warning without this but it works

public:
    float getSpeed() const;
    void setSpeed(float value);

private:
    float mSpeed { 0.f };
};

#endif // SCRIPT_H

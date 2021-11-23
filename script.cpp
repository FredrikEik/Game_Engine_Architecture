#include "script.h"

Script::Script(QObject *parent) : QObject(parent)
{

}

float Script::getSpeed() const
{
    return mSpeed;
}

void Script::setSpeed(float value)
{
    mSpeed = value;
}

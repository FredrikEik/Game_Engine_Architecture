#include "script.h"

Script::Script(QObject *parent) : QObject(parent)
{

}

/**
 * @brief Script::getSpeed
 * @return
 */
float Script::getSpeed() const
{
    return mSpeed;
}

/**
 * @brief Script::setSpeed
 * @param value
 */
void Script::setSpeed(float value)
{
    mSpeed = value;
}

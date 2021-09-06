#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H
#include <QKeyEvent>
#include "input.h"

class InputComponent
{
public:
    virtual ~InputComponent() {}
    virtual void update();//(Player& player) = 0;
};

#endif // INPUTCOMPONENT_H

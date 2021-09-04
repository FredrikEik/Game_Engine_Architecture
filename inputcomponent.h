#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H
#include "input.h"

class InputComponent
{
public:
    virtual ~InputComponent() {}
    virtual void update(/*sende inn spiller*/);


};

#endif // INPUTCOMPONENT_H

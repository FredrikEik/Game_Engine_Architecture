#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H
#include "input.h"

class InputComponent
{
public:
    virtual ~InputComponent();

    float mCameraSpeed{0.05f};
    float mCameraRotateSpeed{0.1f};
    float mPlayerSpeed{0.1f};

    int mMouseXlast{0};
    int mMouseYlast{0};


};

#endif // INPUTCOMPONENT_H

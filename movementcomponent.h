#ifndef MOVEMENTCOMPONENT_H
#define MOVEMENTCOMPONENT_H

#include "input.h"

struct MovementComponent
{
    float mCameraSpeed{0.05f};
    float mCameraRotateSpeed{0.1f};
    float mPlayerSpeed{0.05f};
    int mMouseXlast{0};
    int mMouseYlast{0};
};

#endif // MOVEMENTCOMPONENT_H

#ifndef MOVEMENTCOMPONENT_H
#define MOVEMENTCOMPONENT_H
#include "input.h"

class MovementComponent
{
public:
    virtual ~MovementComponent();

    float mCameraSpeed{0.05f};
    float mCameraRotateSpeed{0.1f};
    float mPlayerSpeed{0.05f};

    int mMouseXlast{0};
    int mMouseYlast{0};

    int EposX{0};
    int EposZ{0};

    int PposX{0};
    int PposZ{0};



};

#endif // MOVEMENTCOMPONENT_H

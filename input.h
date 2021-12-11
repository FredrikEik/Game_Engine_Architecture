#ifndef INPUT_H
#define INPUT_H

/**
    This struct defines the keys we want usually want to use.
    You have to set the state of this table manually
 */
struct Input
{
    bool W{false};
    bool A{false};
    bool S{false};
    bool D{false};
    bool UP{false};     //Arrow keys
    bool DOWN{false};
    bool LEFT{false};
    bool RIGHT{false};
    bool Q{false};
    bool E{false};
    bool C{false};
    bool F{false};      //used to "find" objects in editor
    bool LSHIFT{false};
    bool LCTRL{false};
    bool SPACE{false};
    bool LMB{false};    //Mouse buttons
    bool RMB{false};
    bool MMB{false};
    float MWHEEL{0.f};  //MouseWheel
    int MOUSEX{0};
    int MOUSEY{0};
};

#endif // INPUT_H

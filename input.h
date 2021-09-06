#ifndef INPUT_H
#define INPUT_H

struct Input
{
    //Camera Controlls
    bool W{false};
    bool A{false};
    bool S{false};
    bool D{false};
    bool Q{false};
    bool E{false};

    //Interactive object controlls
    bool UP{false};
    bool DOWN{false};
    bool RIGHT{false};
    bool LEFT{false};

    //Misc controlls
    bool C{false};
    bool X{false};
    bool Z{false};
    bool SHIFT{false};
    bool CTRL{false};
    bool SPACE{false};

    //Mouse
    bool LMB{false};
    bool RMB{false};
    bool MMB{false};
    float MWHEEL{0.f};
    int MOUSEX{0};
    int MOUSEY{0};
};

#endif // INPUT_H

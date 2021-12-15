#ifndef INPUT_H
#define INPUT_H

/**
  Denne structen inneholder informasjon om taste-bruk i runtime.
  Statusen på tastene individuelt håndteres i renderwindow, om de er trykket ned, holdes nede eller er sluppet.
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

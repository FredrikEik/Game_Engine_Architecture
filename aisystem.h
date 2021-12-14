#ifndef AISYSTEM_H
#define AISYSTEM_H
#include "gameobject.h"
#include <vector>

class AISystem
{
public:
    AISystem();
    static bool onHit(GameObject* obj);
    static void updateMovement(GameObject* player, std::vector<GameObject*> objects);

private:

    static float randomNumber(int min, int max, bool negative = false);
};

#endif // AISYSTEM_H

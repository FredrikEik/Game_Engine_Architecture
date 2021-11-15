#ifndef JSONSYSTEM_H
#define JSONSYSTEM_H
#include <iostream>
#include <fstream>
#include <ostream>
#include <istream>
#include "components.h"


class JSONSystem
{
public:
    JSONSystem();
    void JSONSystemInit(class RenderWindow * inRW);
    void SaveLevel();
    void LoadLevel();
    void CheckLevels();

private:
    class RenderWindow * RW;
};

#endif // JSONSYSTEM_H
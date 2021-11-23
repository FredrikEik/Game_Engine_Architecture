#ifndef LASTERRAINGENERATOR_H
#define LASTERRAINGENERATOR_H

#include "gameobject.h"

class LasTerrainGenerator : public GameObject
{
public:
    LasTerrainGenerator();
    ~LasTerrainGenerator();

    void draw();
    void init();
    void move(float x, float y, float z);
    void readFile(std::string filename);
};

#endif // LASTERRAINGENERATOR_H

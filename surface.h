#ifndef SURFACE_H
#define SURFACE_H

#include "visualobject.h"
#include "gameobject.h"


class Surface : public GameObject
{
public:
    Surface();
    Surface(std::string filename);
    ~Surface() override;
    void writeFile(std::string filename);
    void readFile(std::string filename);
    void init() override;
    void draw() override;
};

#endif // SURFACE_H

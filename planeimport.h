#ifndef PLANEIMPORT_H
#define PLANEIMPORT_H

#include "gameobject.h"

class PlaneImport : public GameObject
{
public:
    PlaneImport(std::string filename);
    ~PlaneImport() override;

    virtual void draw() override;
    virtual void init() override;

    std::string filename;

    void readFile(std::string filename);
};

#endif // PLANEIMPORT_H

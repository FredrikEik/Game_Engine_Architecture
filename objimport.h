#ifndef OBJIMPORT_H
#define OBJIMPORT_H

#include "gameobject.h"

class ObjImport : public GameObject
{
public:
    ObjImport(std::string filename);
    ~ObjImport() override;

    virtual void draw() override;
    virtual void init() override;

    std::string filename;

    void readFile(std::string filename);

};

#endif // OBJIMPORT_H

#ifndef MESHHANDLER_H
#define MESHHANDLER_H

#include <QOpenGLFunctions_4_1_Core>

#include "vertex.h"
//#include "material.h"

#include <string>
#include <vector>

#include <QMatrix4x4>

class MeshComponent;


class MeshHandler : public QOpenGLFunctions_4_1_Core
{
public:
    MeshHandler();
    ~MeshHandler();

    void draw();
    void init(MeshComponent &MeshComp);

    void readFile(std::string filename, MeshComponent *MeshComp);

private:


};

#endif // MESHHANDLER_H

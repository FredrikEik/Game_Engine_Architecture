#ifndef SKYBOX_H
#define SKYBOX_H
#include "objreader.h"
#include "gameobject.h"
#include <QOpenGLFunctions_4_1_Core>

class Skybox : public GameObject
{
/**
    \brief class for making the skybox
    \author Paal Marius Haugen
    \date 15/12/21
*/
public:
    ///Constructs the class by running the readFile function
    Skybox(std::string filename);
    ~Skybox() override;

    void draw() override;
    void init() override;
    void move(float x, float y, float z) override;
    ///Leser inn obj fila for skyboxen
    void readFile(std::string filename);
};

#endif // SKYBOX_H

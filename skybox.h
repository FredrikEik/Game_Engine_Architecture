#ifndef SKYBOX_H
#define SKYBOX_H
#include "objreader.h"
#include "gameobject.h"


class Skybox : public GameObject
{
public:
    Skybox(std::string filename);
    ~Skybox() override;

    void init(GLint matrixUniform[4]);
    void readFile(std::string filename);
    void draw();
};

#endif // SKYBOX_H

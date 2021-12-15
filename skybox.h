#ifndef SKYBOX_H
#define SKYBOX_H
#include "objreader.h"
#include "gameobject.h"
#include <QOpenGLFunctions_4_1_Core>



class Skybox : public GameObject
{
public:
    Skybox(std::string filename);
    ~Skybox() override;

    void draw() override;
    void init() override;
    //void init(GLint matrixUniform[4]);
    void move(float x, float y, float z) override;
    void readFile(std::string filename);
};

#endif // SKYBOX_H

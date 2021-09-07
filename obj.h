#ifndef OBJ_H
#define OBJ_H
#include "visualobject.h"
#include <QOpenGLFunctions_4_1_Core>

class Obj : public VisualObject
{
public:
    Obj(std::string filename);
    ~Obj() override;

   // virtual void draw() override;
    void init(/*GLint matrixUniform*/);

private:
    void readFile(std::string filename);

};

#endif // OBJ_H

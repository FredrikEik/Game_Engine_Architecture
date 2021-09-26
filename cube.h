#ifndef CUBE_H
#define CUBE_H

#include <visualobject.h>

class cube : public VisualObject
{
public:
    cube();
    ~cube() override;
    void init(GLint matrixUniform);
    void draw();
};

#endif // CUBE_H

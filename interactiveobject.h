#ifndef INTERACTIVEOBJECT_H
#define INTERACTIVEOBJECT_H

#include "visualobject.h"

class InteractiveObject : public VisualObject
{
public:
    InteractiveObject();
    InteractiveObject(const std::string &filename);

    virtual void init(GLint MatrixUniform) override;
    virtual void draw() override;
    virtual void move(QVector3D inVector);

};

#endif // INTERACTIVEOBJECT_H

#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "components.h"

class VisualObject : public QOpenGLFunctions_4_1_Core {
public:
    VisualObject();
    virtual ~VisualObject();
    virtual void init() = 0;
    virtual void draw() = 0;

    std::string mName;
    std::vector<Components*> mComponents;
};
#endif // VISUALOBJECT_H


#ifndef XYZ_H
#define XYZ_H

#include <QOpenGLFunctions_4_1_Core>
#include "GameObject.h"

class XYZ : public GameObject
{
public:
   XYZ();

   void init() override;
   void draw() override;
};

#endif // XYZ_H

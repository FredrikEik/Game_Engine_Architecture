#ifndef XYZ_H
#define XYZ_H

#include <QOpenGLFunctions_4_1_Core>
#include "visualobject.h"
#include "gameobject.h"

class XYZ : public GameObject
{
public:
   XYZ();
   ~XYZ() override;

   void init() override;
   void draw() override;

private:
       MeshComponent* MeshComp = nullptr;
};

#endif // XYZ_H

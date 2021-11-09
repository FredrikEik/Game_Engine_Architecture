#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include "math.h"
#include "matrix3x3.h"
#include "matrix4x4.h"
#include "vector2d.h"
#include "vector3d.h"
#include "math_constants.h"
#include "constants.h"
#include "gltypes.h"
#include "altypes.h"
#include <string>
#include <QJsonObject>

class scriptsystem
{
public:
    scriptsystem();

    void readData(std::string scriptName);

    void setData(MAX_ENTITIES_TYPE entityID, std::string scriptName);

    QJsonObject mScriptEngine;
};

#endif // SCRIPTSYSTEM_H

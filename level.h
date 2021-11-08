#ifndef LEVEL_H
#define LEVEL_H

#include "factory.h"
#include "gsl/vector3d.h"
#include <vector>

//Level scripted from javascript
class Level
{

public:
    Level();

    void write(QJsonObject &json);

    void read(const QJsonObject &json);

    std::multimap<std::string, gsl::Vector3D> objectsInLevel;

};

#endif // LEVEL_H

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

    void saveLevelAs(QString levelName, std::multimap<std::string, struct SpawnSettings> objectMap);
    void loadLevel(QString fileName);
    void write(QJsonDocument document, QString fileName) const;
    void read(const QJsonObject &json);

    std::multimap<std::string, struct SpawnSettings> objectsInLevel;

};
struct SpawnSettings
{
    gsl::Vector3D initialPos;
    gsl::Vector3D initialScale;
    gsl::Vector3D initialRot;
};

#endif // LEVEL_H

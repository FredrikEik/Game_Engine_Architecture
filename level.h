#ifndef LEVEL_H
#define LEVEL_H

#include "factory.h"
#include "gsl/vector3d.h"
#include <vector>

//Level loaded from JSON
class Level
{

public:
    Level();

    void saveLevelAs(QString levelName, std::multimap<gsl::objectTypes, struct SpawnSettings> objectMap);
    void loadLevel(QString fileName);
    void write(QJsonDocument document, QString fileName) const;
    void read(const QJsonObject &json);

    std::multimap<gsl::objectTypes, struct SpawnSettings> objectsInLevel;

};
struct SpawnSettings
{
    gsl::Vector3D initialPos;
    gsl::Vector3D initialScale;
    gsl::Vector3D initialRot;
};

#endif // LEVEL_H

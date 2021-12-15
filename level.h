#ifndef LEVEL_H
#define LEVEL_H

#include "factory.h"
#include "gsl/vector3d.h"
#include "constants.h"
#include <vector>

///
///    \brief  Class for saving and loading levels. Uses .json.
///    \author Thobias G Halse Ovastrøm
///    \date   15/12/21
///
class Level
{
public:
    Level();

    ///Saves all objects in the scene to a .json level file
    void saveLevelAs(QString levelName, std::multimap<gsl::ObjectType, struct SpawnSettings> objectMap);

    ///Loads all objects from a .json level file to a level class
    void loadLevel(QString fileName);

    void write(QJsonDocument document, QString fileName) const;
    void read(const QJsonObject &json);

    std::multimap<gsl::ObjectType, struct SpawnSettings> objectsInLevel;

};
/// A struct that holds initial spawn position, scale and rotation of an object
struct SpawnSettings
{
    gsl::Vector3D initialPos;
    gsl::Vector3D initialScale;
    gsl::Vector3D initialRot;
};

#endif // LEVEL_H

#ifndef LEVEL_H
#define LEVEL_H

#include "factory.h"
#include "gsl/vector3d.h"
#include <vector>
#include <QObject>

//Level scripted from javascript
class Level : public QObject
{
    Q_OBJECT

public:
    Level(QObject *parent = nullptr);
    Level(Factory& factory);

    Q_INVOKABLE void spawnObjectAtLocation(std::string objectName, float x, float y, float z);
    Factory* mFactory;


};

#endif // LEVEL_H

#include "level.h"
#include <QJsonObject>
#include <QJsonArray>

Level::Level()
{

}

void Level::read(const QJsonObject &json)
{
    if(json.contains("Objects") && json["Objects"].isArray())
    {
        QJsonArray objectArray = json["Objects"].toArray();
        for (int i = 0; i < objectArray.size(); i++)
        {
            qDebug() << objectArray.size();
            QJsonObject jsonObject = objectArray[i].toObject();
            if(jsonObject.contains("objectType") && jsonObject.contains("spawnPosition"))
            {
                if(jsonObject["objectType"].isString() && jsonObject["spawnPosition"].isArray())
                {
                    std::string objectType = jsonObject["objectType"].toString().toStdString();
                    QJsonArray posArray = jsonObject["spawnPosition"].toArray();
                    gsl::Vector3D spawnLocation;
                    spawnLocation.x = posArray[0].toDouble();
                    spawnLocation.y = posArray[1].toDouble();
                    spawnLocation.z = posArray[2].toDouble();
                    objectsInLevel.insert(std::pair<std::string, gsl::Vector3D>(objectType, spawnLocation));
                }
            }
        }
    }
}

#include "level.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

Level::Level()
{

}

void Level::saveLevelAs(QString levelName, std::multimap<std::string, SpawnSettings> objectMap)
{
    QString fileName{"../GEA2021/Saves/" + levelName + ".json"};
    QJsonDocument newLevel;
    QJsonObject jsonObj;
    QJsonArray jsonArray;
    for(auto it = objectMap.begin(); it != objectMap.end(); it++)
    {
        qDebug() << "loop iterator";
        QJsonObject temp;

        std::string objType = it->first;
        QString QObjType = QString::fromStdString(objType);

        temp.insert("objectType", QObjType);
        temp.insert("position", QJsonArray{it->second.initialPos.x,it->second.initialPos.y,it->second.initialPos.z});
        temp.insert("scale", QJsonArray{it->second.initialScale.x,it->second.initialScale.y,it->second.initialScale.z});
        temp.insert("rotation", QJsonArray{it->second.initialRot.x,it->second.initialRot.y,it->second.initialRot.z});

        jsonArray.push_back(temp);
    }
    jsonObj.insert("Objects", jsonArray);
    newLevel.setObject(jsonObj);
    write(newLevel, fileName);
}

void Level::write(QJsonDocument document, QString fileName) const
{
    QFile jsonFile(fileName);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(document.toJson());
}

void Level::loadLevel(QString fileName)
{
    QFile loadFile((QString(fileName)));
    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not load level";
        return;
    }
    QByteArray saveData = loadFile.readAll();   //read whole file
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));   //convert to json document
    QJsonObject jsonObject = loadDoc.object();  //read first object == whole thing
    read(jsonObject);
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
            SpawnSettings settings;

            std::string objectType;
            gsl::Vector3D spawnPos;
            gsl::Vector3D spawnRot;
            gsl::Vector3D spawnScale;


            //Gets objectType
            if(jsonObject.contains("objectType") && jsonObject["objectType"].isString())
            {
                objectType = jsonObject["objectType"].toString().toStdString();
            }
            else
            {
                objectType = "Cube";
            }

            //Gets spawn position
            if(jsonObject.contains("position") && jsonObject["position"].isArray())
            {
                QJsonArray posArray = jsonObject["position"].toArray();
                settings.initialPos.x = posArray[0].toDouble();
                settings.initialPos.y = posArray[1].toDouble();
                settings.initialPos.z = posArray[2].toDouble();
            }
            else
            {
                spawnPos = gsl::Vector3D(0,0,0);
            }

            //Gets spawn scale
            if(jsonObject.contains("scale") && jsonObject["scale"].isArray())
            {
                QJsonArray scaleArray = jsonObject["scale"].toArray();
                settings.initialScale.x = scaleArray[0].toDouble();
                settings.initialScale.y = scaleArray[1].toDouble();
                settings.initialScale.z = scaleArray[2].toDouble();
            }
            else
            {
                spawnScale = gsl::Vector3D(1,1,1);
            }

            //Gets spawn rotation
            if(jsonObject.contains("rotation") && jsonObject["rotation"].isArray())
            {
                QJsonArray rotArray = jsonObject["rotation"].toArray();
                settings.initialRot.x = rotArray[0].toDouble();
                settings.initialRot.y = rotArray[1].toDouble();
                settings.initialRot.z = rotArray[2].toDouble();
            }
            else
            {
                spawnRot = gsl::Vector3D(0,0,0);
            }

            objectsInLevel.insert(std::pair<std::string, struct SpawnSettings>(objectType, settings));
        }
    }
}

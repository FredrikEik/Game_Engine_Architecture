#include "heigthmap.h"


HeigthMap::HeigthMap()
{

}

void HeigthMap::ReadLasFile()
{
    float xMin, xMax, yMin, yMax;
    int resolution;

    std::ifstream inLasFile;

    inLasFile.open(gsl::ProjectFolderName + "Assets/test_las.txt");

    if(!inLasFile)
    {
        qDebug() << "unable to open file test_las.txt";
    }

    std::vector<gsl::Vector3D> pointData;
    int lineSize;
    float x, y, z = {0};

    if(inLasFile)
    {
         inLasFile >> lineSize;

         for(int i = 0; i < lineSize; i++)
         {
             inLasFile >> x >> z >> y;

             pointData.emplace_back(x, y, z);
         }
    }


    qDebug() << "inLasFile lineSize" << lineSize << "PointData Size" << pointData.size();

    for (int printNr = 0; printNr < 5; printNr++)
    {
        qDebug() << pointData[printNr];
    }
}

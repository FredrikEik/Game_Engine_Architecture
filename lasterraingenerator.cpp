#include "lasterraingenerator.h"

LasTerrainGenerator::LasTerrainGenerator()
{

}
~LasTerrainGenerator::LasTerrainGenerator()
{

}
void draw()
{

}
void init()
{

}
void move(float x, float y, float z)
{

}


void LasTerrainGenerator::readFile(std::string filename)
{

    std::vector<gsl::Vector3D> mAllDataPoints;

    float x,y,z;

    float xMax;
    float yMax;
    float zMax;

    float xMin;
    float yMin;
    float zMin;

    unsigned int n;
    bool once = true;

    std::ifstream inn;
    inn.open(filename.c_str());

    if(inn.is_open())
    {
        gsl::Vector3D pos;
        inn>>n;
        int loopLenth = n;
        //        float i{0};
        mAllDataPoints.reserve(n);



        for(int i{0}; i<loopLenth;i++)
        {
            inn >> x;
            inn >> z;
            inn >> y;

            pos.setX(x);
            pos.setZ(z);
            pos.setY(y);

            mAllDataPoints.push_back(pos);


            if(once)
            {
                xMin = x;
                xMax = x;
                zMin = z;
                zMax = z;
                yMin = y;
                yMax = y;
                once = false;
            }
            if(x < xMin)
                xMin = x;

            if(x > xMax)
                xMax = x;

            if(z < zMin)
                zMin = z;

            if(z > zMax)
                zMax = z;

            if(y < yMin)
                yMin = y;

            if(y > yMax)
                yMax = y;

        }
        inn.close();

        for(unsigned long long i = 0; i < loopLenth;i++)
        {
            mAllDataPoints[i].setX(mAllDataPoints[i].getX() - xMin);
            mAllDataPoints[i].setZ(mAllDataPoints[i].getZ() - zMin);
            mAllDataPoints[i].setY(mAllDataPoints[i].getY() - yMin);
        }
    }
    xMax = xMax-xMin;
    xMin = xMin-xMin;
    zMax = zMax-zMin;
    zMin = zMin-zMin;
    yMax = yMax-yMin;
    yMin = yMin-yMin;



}

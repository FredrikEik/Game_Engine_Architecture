#include "lasheightmap.h"
#include "lasheightmap.h"
#include <qdebug.h>

LASHeightMap::LASHeightMap(std::string fileName)
{
    ReadDatafromFile(fileName);
    //make a array data thingie
    populatePosArr();
    GenerateHeightMap();
}

void LASHeightMap::ReadDatafromFile(std::string fileName)
{
    //get data from spesific LAS file :D
    std::string line;
    std::ifstream myfile (fileName);


    std::string x,y,z;
    if (myfile.is_open())
    {
        getline (myfile,line);
        std::cout <<"SIZE OF LIST: "<< line << '\n';
        while ( getline (myfile,line) )
        {
            //getting x positions
            x += line[0];
            x += line[1];
            x += line[2];
            x += line[3];
            x += line[4];
            x += line[5];
            x += line[6];
            x += line[7];
            x += line[8];

            //getting y positions
            y += line[10];
            y += line[11];
            y += line[12];
            y += line[13];
            y += line[14];
            y += line[15];
            y += line[16];
            y += line[17];
            y += line[18];
            y += line[19];
            //getting z positions
            z += line[21];
            z += line[22];
            z += line[23];
            z += line[24];
            z += line[25];
            z += line[26];

            //std::cout <<x<< " "<< y << " "<<z<< '\n';
            positions.push_back(Vertex(std::stof(x),std::stof(z),std::stof(y), (line[4] - '0')*0.1f ,(line[23]- '0')*0.1f ,(line[16]- '0')*0.1f ,0 ,0)); // setting data into Vertex
            x = "";
            y = "";
            z = "";

        }
        myfile.close();
        //delete delta in the vertex, as it starts with insaneley big values :D
        RemoveDeltaPos();
    }
    else
    {
        QString string = QString::fromStdString( fileName);
        qDebug() << "LASHEIGHTMAP ______ Unable to open file: " << string;
    }



}



void LASHeightMap::populatePosArr()
{
    qDebug() << "WE ARE HERE";
    //makes empty populated array
    for(int i = 0; i < 999 ; i++)
    {
        for (int j = 0 ; j < 1999; j++ )
            PosArr[i][j] = 0.0f;
    }

    qDebug() << "WE ARE istiatiaat";
    for(unsigned long long i = 0; i < positions.size(); i++)
    {
        int X =static_cast<int> (positions[i].getVertex().getX());
        int Z =static_cast<int> (positions[i].getVertex().getZ());

        PosArr[X][Z] = positions[i].getVertex().getY();//(PosArr[X][Z] +  positions[i].getVertex().getY())/2.0f;
        //qDebug() << (positions[i].getVertex());

    }
    qDebug() << "WE ARE THERE" << PosArr[300][100];
}

std::vector<Vertex> LASHeightMap::getPositions() const
{
    return positions;
}
std::vector<Vertex> LASHeightMap::getmVertices() const
{
    return mVertices;
}

void LASHeightMap::setPositions(const std::vector<Vertex> &value)
{
    positions = value;
}

void LASHeightMap::RemoveDeltaPos()
{
    float smallestX{0}, biggestX{0};
    float smallestY{0}, biggestY{0};
    float smallestZ{0}, biggestZ{0};

    //we take biggest delta between x,y and z, then we remove it from the vertexes to get a more appropreate number.
    for(auto i = 0; i<positions.size(); i++)
    {

        gsl::Vector3D temp = positions[i].getVertex();
        //qDebug() << "xyz "<< temp;
        //check size of x
        if(temp.getX() < smallestX)
            smallestX = temp.getX();
        if(temp.getX() > biggestX)
            biggestX = temp.getX();
        //check size of y
        if(temp.getY() < smallestY)
            smallestY = temp.getY();
        if(temp.getY() > biggestY)
            biggestY = temp.getY();
        //check size of Z
        if(temp.getZ() < smallestZ)
            smallestZ = temp.getZ();
        if(temp.getZ() > biggestZ)
            biggestZ = temp.getZ();
    }

    float deltaX{biggestX - smallestX}, deltaY{biggestY - smallestY }, deltaZ{biggestZ - smallestZ };
    qDebug() << "Delta x : "<<deltaX<< " Delta y: " << deltaY<< " Delta z: "<< deltaZ<< "\n";
    qDebug() << "Biggest x : "<< biggestX<< " smallest x: " << smallestX<< "\n";
    qDebug() << "Biggest y : "<<biggestY<< " smallest Y: " << smallestY<< "\n";
    qDebug() << "Biggest z : "<<biggestZ<< " smallest z: " << smallestZ<< "\n";


    float smallestX1{0}, biggestX1{0};
    float smallestY1{0}, biggestY1{0};
    float smallestZ1{0}, biggestZ1{0};
    for(auto i = 0; i<positions.size(); i++)
    {
        positions[i].set_xyz(deltaX - positions[i].getVertex().getX() , deltaY - positions[i].getVertex().getY() ,   deltaZ - positions[i].getVertex().getZ());
        //qDebug() << positions[i].getVertex();//.getX() << positions[i].getVertex().getY() << positions[i].getVertex().getZ() ;
    }


    //    for(auto i = 0; i<positions.size(); i++)
    //    {
    //        //positions[i].set_xyz(gsl::Vector3D(positions[i].getVertex().getX() - deltaX, positions[i].getVertex().getY() - deltaY, positions[i].getVertex().getZ() - deltaZ));
    //        qDebug() << positions[i].getVertex();
    //    }
}

void LASHeightMap::GenerateHeightMap()
{
    float f = 1;
    for(float x = 0; x<900; x+=1)
        for(float z =0; z<1000; z+=1)
        {

            float height1 = CalcHeight(    x,    z);
            float height2 = CalcHeight(  x+1,    z);
            float height3 = CalcHeight(    x,  z+1);
            float height4 = CalcHeight(    x,  z+1);
            float height5 = CalcHeight(  x+1,    z);
            float height6 = CalcHeight(  x+1,  z+1);

            mVertices.push_back(Vertex{  x, height1,   z,  height1/30, height1/20, height1/20,0,0});
            mVertices.push_back(Vertex{x+1, height2,   z,  height2/30, height2/20, height2/20,0,0});
            mVertices.push_back(Vertex{  x, height3, z+1,  height3/30, height3/20, height3/20,0,0});
            mVertices.push_back(Vertex{  x, height4, z+1,  height4/30, height4/20, height4/20,0,0});
            mVertices.push_back(Vertex{x+1, height5,   z,  height5/30, height5/20, height5/20,0,0});
            mVertices.push_back(Vertex{x+1, height6, z+1,  height6/30, height6/20, height6/20,0,0});
        }
}

float LASHeightMap::CalcHeight(float x, float z)
{
    float height = 10.0f;

    QVector2D xzVec = QVector2D(x,z); // here is pos
    QVector2D tempPos, closest;
    float shortestLength{1};
    //find closest tempX or Z
    /*for(unsigned long long i = 0; i < positions.size(); i++)
    {
        //calculate legth between point and square
        QVector2D tempLength;
        tempPos = QVector2D(positions[i].getVertex().getX(), positions[i].getVertex().getZ() );
        tempLength = tempPos - xzVec;
        float length = tempPos.distanceToPoint(xzVec);


        if(length < shortestLength && length > 0.01f)
        {
            shortestLength = length;
            closest = tempPos;
            height = positions[i].getVertex().getY();
            break;
        }
        //if(shortestLength <0.3f && shortestLength > 0.0f)
         //   break;

        //qDebug() << positions[i].getVertex();//.getX() << positions[i].getVertex().getY() << positions[i].getVertex().getZ() ;
    }*/

    int X = static_cast<int>(x);
    int Z = static_cast<int>(z);
    int counter =0;
    float collected = 0;
    height = PosArr[X][Z];

    if((X>10 && Z>10) /*&& height <= 2.0f */)
    {

        for(int i = -5; i < 5 ; i++)
        {
            for (int j = -5 ; j <5; j++ )
            {
                if(PosArr[X + i][Z + j] != 0)
                {
                    collected += PosArr[X + i][Z + j];
                    counter++;
                }
            }
        }
        height = ( collected)/(counter);
        //average height of surrounding area
    }
    else
    {
        height = PosArr[X][Z] ;
    }


    //do the average calc
    //qDebug() << height;
    //then return its
    return height;
}

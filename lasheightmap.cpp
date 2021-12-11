#include "lasheightmap.h"
#include "lasheightmap.h"
#include <qdebug.h>
#include <algorithm>

LASHeightMap::LASHeightMap(std::string fileName, int inResolution)
{
    ReadDatafromFile(fileName);
    //make a array data thingie
    Resolution = inResolution;
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

std::vector<Vertex> LASHeightMap::getCountourPoints() const
{
    return CountourPoints;
}

void LASHeightMap::setCountourPoints(const std::vector<Vertex> &value)
{
    CountourPoints = value;
}



void LASHeightMap::populatePosArr()
{

    //makes empty populated array
    for(int i = 0; i < 999 ; i++)
    {
        for (int j = 0 ; j < 1999; j++ )
            PosArr[i][j] = 0.0f;
    }


    for(unsigned long long i = 0; i < positions.size(); i++)
    {
        int X =static_cast<int> (positions[i].getVertex().getX());
        int Z =static_cast<int> (positions[i].getVertex().getZ());

        PosArr[X][Z] = positions[i].getVertex().getY();//(PosArr[X][Z] +  positions[i].getVertex().getY())/2.0f;
        //qDebug() << (positions[i].getVertex());

    }

}

std::vector<Vertex> LASHeightMap::getPositions() const
{
    return positions;
}

std::vector<Vertex> LASHeightMap::getmVertices() const
{
    return mVertices;
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



    for(auto i = 0; i<positions.size(); i++)
    {
        positions[i].set_xyz(deltaX - positions[i].getVertex().getX() , deltaY - positions[i].getVertex().getY() ,   deltaZ - positions[i].getVertex().getZ());
        //qDebug() << positions[i].getVertex();//.getX() << positions[i].getVertex().getY() << positions[i].getVertex().getZ() ;
    }


}

void LASHeightMap::GenerateHeightMap()
{
    float ofsetx = 0;
    float ofsetz = 0;
    float ofsety = 0;

    for(float x = 50; x<X-1; x+=Resolution)//(float x = 100; x<150; x+=1)// 300/400
        for(float z =50; z<Z-1; z+=Resolution)//(float z =100; z<150; z+=1)

        {
            //get all height data :D
            float height1 =  CalcHeight(    x  ,    z);
            float height2 = CalcHeight(  x+Resolution,    z);
            float height3 = CalcHeight(    x  ,  z+Resolution);
            float height4 = CalcHeight(    x,  z+Resolution);
            float height5 = CalcHeight(  x+Resolution,    z);
            float height6 = CalcHeight(  x+Resolution,  z+Resolution);
            //use height date for colouring   //This order is like this because our
            mVertices.push_back(Vertex{ofsetx +    x  , (ofsety + height1), ofsetz +   z  ,       height1/100, height1/100, height1/100,0,0}); //1
            mVertices.push_back(Vertex{ofsetx + x+ Resolution, (ofsety + height2), ofsetz +   z  ,       height2/100, height2/100, height2/100,0,0}); //2
            mVertices.push_back(Vertex{ofsetx +    x  , (ofsety + height3), ofsetz + z+Resolution,       height3/100, height3/100, height3/100,0,0}); //3
            mVertices.push_back(Vertex{ofsetx +    x  , (ofsety + height4), ofsetz + z+Resolution,       height4/100, height4/100, height4/100,0,0}); //4
            mVertices.push_back(Vertex{ofsetx + x+ Resolution, (ofsety + height5), ofsetz +   z  ,       height5/100, height5/100, height5/100,0,0}); //5
            mVertices.push_back(Vertex{ofsetx + x+ Resolution, (ofsety + height6), ofsetz + z+Resolution,       height6/100, height6/100, height6/100,0,0}); //6

            //contour line collector
            mHPoints[static_cast<int>(x)][static_cast<int>(z)] = height1;


        }

}

float LASHeightMap::CalcHeight(float x, float z)
{
    float height = 0.0f;

    int resolution =15; // 15 default: this resolution on the size of average height collector
    int X = static_cast<int>(x);
    int Z = static_cast<int>(z);
    int counter =0;
    float collected = 0;
    height = PosArr[X][Z];

    if((X>resolution && Z>resolution) /*&& height <= 2.0f */) //are we taking averages of things out of bounds of avg collector?
    {
        for(int i = -resolution - 1; i < resolution - 1; i++)
        {
            for (int j = -resolution - 1; j <resolution - 1; j++ )//double for loop to make a square that collects all the heights
            {
                if(PosArr[X + i][Z + j] != 0)
                {
                    collected += PosArr[X + i][Z + j];
                    counter++;
                }
            }
        }
        //average height of surrounding area
        height = ( collected)/(counter);
    }
    else // if wer are out of scope, just add heights
    {
        height = PosArr[X][Z] ;
    }


    //we find the highest and lowest y values


    //check size of y
    if(height < lowestY && height>20.f)
        lowestY =height;

    if(height > heighestY)
     heighestY = height;



    //then return its
    return -height + 90; //changed dirrecton to flip the map + 90 offset (can make it dynamic if we want)
}



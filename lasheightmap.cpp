#include "lasheightmap.h"
#include <qdebug.h>

LASHeightMap::LASHeightMap(std::string fileName)
{
    ReadDatafromFile(fileName);
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
            positions.push_back(Vertex(std::stof(x),std::stof(z),std::stof(y), 0,0 ,0 ,0 ,0)); // setting data into Vertex
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

    float deltaX{biggestX - smallestX}, deltaY{biggestY - smallestY}, deltaZ{biggestZ - smallestZ};
    qDebug() << "Delta x : "<<deltaX<< " Delta y: " << deltaY<< " Delta z: "<< deltaZ<< "\n";

    for(auto i = 0; i<positions.size(); i++)
    {
        positions[i].set_xyz(deltaX - positions[i].getVertex().getX() , deltaY - positions[i].getVertex().getY() ,   deltaZ - positions[i].getVertex().getZ());
    }

//    for(auto i = 0; i<positions.size(); i++)
//    {
//        //positions[i].set_xyz(gsl::Vector3D(positions[i].getVertex().getX() - deltaX, positions[i].getVertex().getY() - deltaY, positions[i].getVertex().getZ() - deltaZ));
//        qDebug() << positions[i].getVertex();
//    }
}

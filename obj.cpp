//#include "obj.h"
//#include "vertex.h"
//#include "component.h"
//#include <sstream>
//#include <fstream>
//#include <string>

//Obj::Obj(std::string filename) : VisualObject ()
//{
//    mTransform = new TransformComponent();
//    mTransform->mMatrix.setToIdentity();

//    mMesh = new MeshComponent();

//    readFile(filename);

//    mMesh->mDrawType = GL_TRIANGLES;

//    mMaterial = new MaterialComponent();

//}

//Obj::~Obj()
//{

//}


//void Obj::readFile(std::string filename)
//{
//    //Open File
//    //    std::string filename = Orf::assetFilePath.toStdString() + fileName + ".obj";
//    std::ifstream fileIn;
//    fileIn.open (filename, std::ifstream::in);
//    if(!fileIn)
//        qDebug() << "Could not open file for reading: " << QString::fromStdString(filename);

//    //One line at a time-variable
//    std::string oneLine;
//    //One word at a time-variable
//    std::string oneWord;

//    std::vector <gsl::Vector3D> tempVertecies;
//    std::vector <gsl::Vector3D> tempNormals;
//    std::vector <gsl::Vector2D> tempUVs;

//    // Varible for constructing the indices vector
//    unsigned int temp_index = 0;

//    //Reading one line at a time from file to oneLine
//    while(std::getline(fileIn, oneLine))
//    {
//        //Doing a trick to get one word at a time
//        std::stringstream sStream;
//        //Pushing line into stream
//        sStream << oneLine;
//        //Streaming one word out of line
//        oneWord = ""; //resetting the value or else the last value might survive!
//        sStream >> oneWord;

//        if (oneWord == "#")
//        {
//            continue;
//        }
//        if (oneWord == "")
//        {
//            continue;
//        }
//        if (oneWord == "v")
//        {
//            gsl::Vector3D tempVertex;
//            sStream >> oneWord;
//            tempVertex.setX (std::stof(oneWord));
//            sStream >> oneWord;
//            tempVertex.setY(std::stof(oneWord));
//            sStream >> oneWord;
//            tempVertex.setZ  (std::stof(oneWord));

//            tempVertecies.push_back(tempVertex);

//            continue;
//        }
//        if (oneWord == "vt")
//        {
//            gsl::Vector2D tempUV;
//            sStream >> oneWord;
//            tempUV.setX  (std::stof(oneWord));
//            sStream >> oneWord;
//            tempUV.setY  (std::stof(oneWord));

//            //UV made - pushing it into UV-vector
//            tempUVs.push_back(tempUV);

//            continue;
//        }
//        if (oneWord == "vn")
//        {

//            gsl::Vector3D tempNormal;
//            sStream >> oneWord;
//            tempNormal.setX (std::stof(oneWord));
//            sStream >> oneWord;
//            tempNormal.setY  (std::stof(oneWord));
//            sStream >> oneWord;
//            tempNormal.setZ (std::stof(oneWord));

//            //Vertex made - pushing it into vertex-vector
//            tempNormals.push_back(tempNormal);
//            continue;
//        }
//        if (oneWord == "f")
//        {
//            int index, normal, uv;
//            for(int i = 0; i < 3; i++)
//            {
//                sStream >> oneWord;     //one word read
//                std::stringstream tempWord(oneWord);    //to use getline on this one word
//                std::string segment;    //the numbers in the f-line
//                std::vector<std::string> segmentArray;  //temp array of the numbers
//                while(std::getline(tempWord, segment, '/')) //splitting word in segments
//                {
//                    segmentArray.push_back(segment);
//                }
//                index = std::stoi(segmentArray[0]);     //first is vertex
//                if (segmentArray[1] != "")              //second is uv
//                    uv = std::stoi(segmentArray[1]);
//                else
//                {
//                    //qDebug() << "No uvs in mesh";       //uv not present
//                    uv = 0;                             //this will become -1 in a couple of lines
//                }
//                normal = std::stoi(segmentArray[2]);    //third is normal

//                //Fixing the indexes
//                //because obj f-lines starts with 1, not 0
//                --index;
//                --uv;
//                --normal;

//                if (uv > -1)    //uv present!
//                {
//                    Vertex tempVert(tempVertecies[index], tempNormals[normal], tempUVs[uv]);
//                    mMesh->mVertices.push_back(tempVert);
//                }
//                else            //no uv in mesh data, use 0, 0 as uv
//                {
//                    Vertex tempVert(tempVertecies[index], tempNormals[normal], gsl::Vector2D(0.0f, 0.0f));
//                    mMesh->mVertices.push_back(tempVert);
//                }
//                mMesh->mIndices.push_back(temp_index++);
//            }
//            continue;
//        }
//    }
//    fileIn.close();
//}

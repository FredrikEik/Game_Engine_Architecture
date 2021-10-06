#include "resourcemanager.h"


ResourceManager::ResourceManager()
{
id = 0;
std::string name = "objName";


}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::manageObjects(std::string objectName, int ObjectID)
{

    //Axis
    //      VisualObject *temp = new XYZ();
    //      temp->mMaterial->mShaderProgram = 0; //plain shader
    //      temp->init();
    //      mVisualObjects.push_back(temp);

    name = objectName;
    addObject(name, id, ObjectID, 1, 1, 1); //name, ID, ObjectType, translate, mesh, material

    //Dag's dog triangle
//      temp = new Triangle();
//      temp->init();
//      temp->mMaterial->mShaderProgram = 1;    //texture shader
//      temp->mMaterial->mTextureUnit = 1;      //dog texture
//      temp->mTransform->mMatrix.translate(0.f, 0.f, .5f);
//      mVisualObjects.push_back(temp);

//    VisualObject *dagsDog;

    //Set up camera
//    Camera *mCurrentCamera;

//    mCurrentCamera = new Camera();
//    mCurrentCamera->setPosition(gsl::Vector3D(1.f, .5f, 4.f));

    id++;
}

void ResourceManager::addObject(std::string name, int id, int ObjectType,
                                int TransformID, int MeshID, int MaterialID)
{
     VisualObject * CurrentVisualObject = 0;

    if(ObjectType == 0) {
        //Axis
//              VisualObject *temp = new XYZ();
//              temp->mMaterial->mShaderProgram = 0; //plain shader
//              temp->init();
//              mVisualObjects.push_back(temp);
        CurrentVisualObject = new XYZ();
    }

    else if(ObjectType == 1) {
        //Triangle
        CurrentVisualObject = new Triangle();
    }

    else if(ObjectType == 2) {
        //Cube
        CurrentVisualObject = new Cube();
    }

    else if (ObjectType == 3) {
        //obj
        readObj(name, CurrentVisualObject);
    }



    mVisualObjects.push_back(CurrentVisualObject);

    CurrentVisualObject->entitiyID = id;
    CurrentVisualObject->mName = name;

    CurrentVisualObject->init();

    if (TransformID == 1)
    {
        new XYZ();
        CurrentVisualObject->mTransform->mMatrix.translate(.001f, .001f, -.001f); //This only moves it for one frame! How do we make it happen every frame?
    }

    if (MeshID != 0)
    {
        //Read from an obj file

    }

    if (MaterialID != 0)
    {
        CurrentVisualObject->mMaterial->mShaderProgram = MaterialID;
        CurrentVisualObject->mMaterial->mTextureUnit = MaterialID;
    }

}

void readObj(std::string name, VisualObject* object)
{
    std::ifstream fileIn;
    fileIn.open (name, std::ifstream::in);
    if(!fileIn)
        qDebug() << "Could not open file for reading: " << QString::fromStdString(name);
    //One line at a time-variable
    std::string oneLine;
    //One word at a time-variable
    std::string oneWord;

    std::vector<gsl::Vector3D> tempVertecies;
    std::vector<gsl::Vector3D> tempNormals;
    std::vector<gsl::Vector2D> tempUVs;

    //    std::vector<Vertex> mVertices;    //made in VisualObject
    //    std::vector<GLushort> mIndices;   //made in VisualObject

    // Varible for constructing the indices vector
    unsigned int temp_index = 0;

    //Reading one line at a time from file to oneLine
    while(std::getline(fileIn, oneLine))
    {
        //Doing a trick to get one word at a time
        std::stringstream sStream;
        //Pushing line into stream
        sStream << oneLine;
        //Streaming one word out of line
        oneWord = ""; //resetting the value or else the last value might survive!
        sStream >> oneWord;

        if (oneWord == "#")
        {
            //Ignore this line
            //            qDebug() << "Line is comment "  << QString::fromStdString(oneWord);
            continue;
        }
        if (oneWord == "")
        {
            //Ignore this line
            //            qDebug() << "Line is blank ";
            continue;
        }
        if (oneWord == "v")
        {
            //            qDebug() << "Line is vertex "  << QString::fromStdString(oneWord) << " ";
            gsl::Vector3D tempVertex;
            sStream >> oneWord;
            tempVertex.x = std::stof(oneWord);
            sStream >> oneWord;
            tempVertex.y = std::stof(oneWord);
            sStream >> oneWord;
            tempVertex.z = std::stof(oneWord);

            //Vertex made - pushing it into vertex-vector
            tempVertecies.push_back(tempVertex);

            continue;
        }
        if (oneWord == "vt")
        {
            //            qDebug() << "Line is UV-coordinate "  << QString::fromStdString(oneWord) << " ";
            gsl::Vector2D tempUV;
            sStream >> oneWord;
            tempUV.x = std::stof(oneWord);
            sStream >> oneWord;
            tempUV.y = std::stof(oneWord);

            //UV made - pushing it into UV-vector
            tempUVs.push_back(tempUV);

            continue;
        }
        if (oneWord == "vn")
        {
            //            qDebug() << "Line is normal "  << QString::fromStdString(oneWord) << " ";
            gsl::Vector3D tempNormal;
            sStream >> oneWord;
            tempNormal.x = std::stof(oneWord);
            sStream >> oneWord;
            tempNormal.y = std::stof(oneWord);
            sStream >> oneWord;
            tempNormal.z = std::stof(oneWord);

            //Vertex made - pushing it into vertex-vector
            tempNormals.push_back(tempNormal);
            continue;
        }
        if (oneWord == "f")
        {
            //            qDebug() << "Line is a face "  << QString::fromStdString(oneWord) << " ";
            //int slash; //used to get the / from the v/t/n - format
            int index, normal, uv;
            for(int i = 0; i < 3; i++)
            {
                sStream >> oneWord;     //one word read
                std::stringstream tempWord(oneWord);    //to use getline on this one word
                std::string segment;    //the numbers in the f-line
                std::vector<std::string> segmentArray;  //temp array of the numbers
                while(std::getline(tempWord, segment, '/')) //splitting word in segments
                {
                    segmentArray.push_back(segment);
                }
                index = std::stoi(segmentArray[0]);     //first is vertex
                if (segmentArray[1] != "")              //second is uv
                    uv = std::stoi(segmentArray[1]);
                else
                {
                    //qDebug() << "No uvs in mesh";       //uv not present
                    uv = 0;                             //this will become -1 in a couple of lines
                }
                normal = std::stoi(segmentArray[2]);    //third is normal

                //Fixing the indexes
                //because obj f-lines starts with 1, not 0
                --index;
                --uv;
                --normal;

                if (uv > -1)    //uv present!
                {
                    Vertex tempVert(tempVertecies[index], tempNormals[normal], tempUVs[uv]);
                    object->mMesh->mVertices.push_back(tempVert);
                }
                else            //no uv in mesh data, use 0, 0 as uv
                {
                    Vertex tempVert(tempVertecies[index], tempNormals[normal], gsl::Vector2D(0.0f, 0.0f));
                    object->mMesh->mVertices.push_back(tempVert);
                }
                object->mMesh->mIndices.push_back(temp_index++);
            }
            continue;
        }
    }

    //being a nice boy and closing the file after use
    fileIn.close();
}

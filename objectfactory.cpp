#include "objectfactory.h"
#include "cube.h"

ObjectFactory::ObjectFactory()
{

}

void ObjectFactory::createObject(std::string objectName)
{
    GameObject* willCreateObject;

    if(objectName == "Cube")
    {
        willCreateObject = new Cube;
    }

<<<<<<< Updated upstream
=======
    else if (objectName == "Triangle")
    {
        willCreateObject = new Triangle;
    }

    else if (objectName == "Goat")
    {
//        willCreateObject = new ObjImport("../GEA2021/Assets/goat.obj");
    }
    else
        return;

//    ObjImport objImport;
//                MeshComponent* newMesh = new MeshComponent();
//                objImport.readFile(fileName, &newMesh->mVertices, &newMesh->mIndices);

>>>>>>> Stashed changes
    willCreateObject->init();
    mGameObject.push_back(willCreateObject);
    return;
}

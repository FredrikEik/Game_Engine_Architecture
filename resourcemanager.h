#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QOpenGLFunctions_4_1_Core>
#include <string>
#include <map>
#include <vector>

#include "components.h"

class ResourceManager : public QOpenGLFunctions_4_1_Core
{
public:
    static ResourceManager& getInstance();

    class GameObject* AddObject(std::string filename);

private:
    ResourceManager();  //singleton
    void operator=(ResourceManager&);           //Assignment operator
    ResourceManager(const ResourceManager&);    //Copy constructor

    int readObj(std::string filename);
    int makeAxis();
    int makeTriangle();

    void initMesh(MeshComponent & tempMeshComp);

    std::vector<MeshComponent> mMeshComponents;

};

#endif // RESOURCEMANAGER_H

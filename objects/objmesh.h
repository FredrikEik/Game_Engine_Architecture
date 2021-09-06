#ifndef OBJMESH_H
#define OBJMESH_H
#include <sstream>

#include "visualobject.h"
#include "systems/ecs/Components.h"

class ObjMesh : public VisualObject
{
public:
    ObjMesh(std::string fileName);
    ~ObjMesh();

    virtual void draw() override;
    virtual void init() override;

    void readFile(std::string fileName);
    void makeVertex();
    void moveAlongCurve(unsigned int increment, std::vector<QVector3D> points, VisualObject* ground);

    std::ifstream objFile;
    std::string oneLine;
    std::string oneWord = "";
    std::stringstream sStream;
    float v1 = 0;
    float v2 = 0;
    float v3 = 0;
    std::vector<QVector3D> importedPos;
    std::vector<QVector3D> importedNorms;
    std::vector<QVector2D> importedUVs;
    size_t OBJi{2};

    int indexPos;
    int indexUVs;
    int indexNorms;
    int indices = 0;

    MeshComponent mMesh;
};

#endif // OBJMESH_H

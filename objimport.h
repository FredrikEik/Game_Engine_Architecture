#ifndef OBJIMPORT_H
#define OBJIMPORT_H

#include "gameobject.h"

class ObjImport : public GameObject
{
public:
    ObjImport();
//    ~ObjImport() override;

//    virtual void draw() override;
//    virtual void init() override;

    std::string filename;

    void readFile(std::string filename, std::vector<Vertex> *mVertices, std::vector<GLuint> *mIndices);

private:

    MeshComponent* MeshComp;
    TextureComponent* TextureComp;

};

#endif // OBJIMPORT_H

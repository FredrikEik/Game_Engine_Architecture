#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "components.h"

class GameObject: public QOpenGLFunctions_4_1_Core {
public:
    GameObject();
    ~GameObject();
    //void init(std::vector<Vertex> vertices);
    void draw();
    void move(float x, float y, float z);

    uint32_t ID;
    std::string mObjectName{"NoName"};  //temp

    TransformComponent* getTransformComponent(){return transformComp;}
    MeshComponent* getMeshComponent(){return meshComp;}
    MaterialComponent* getMaterialComponent(){return materialComp;}
    SphereCollisionComponent* getSphereCollisionComponent() {return sphereCollisionComp;}



    void setMeshComponent(MeshComponent newMeshComp){meshComp=&newMeshComp;}

    //void initMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
    //void initMesh(std::vector<Vertex> vertices);

private:
    TransformComponent* transformComp;
    MeshComponent* meshComp;
    MaterialComponent* materialComp;
    SphereCollisionComponent* sphereCollisionComp;

};
#endif // GAMEOBJECT_H


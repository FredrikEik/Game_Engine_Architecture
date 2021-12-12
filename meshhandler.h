#ifndef MESHHANDLER_H
#define MESHHANDLER_H

#include <QOpenGLFunctions_4_1_Core>

#include "vertex.h"
//#include "material.h"

#include <string>
#include <vector>

#include <QMatrix4x4>

class GameObject;
class MeshComponent;
class CollisionComponent;


class MeshHandler : public QOpenGLFunctions_4_1_Core
{
public:
    MeshHandler();
    ~MeshHandler();

    void draw();
    void init(MeshComponent &MeshComp, int LODlvl);
    void initCollisionBox(MeshComponent &MeshComp, int LODlvl);

    //this takes inn to many arguments...
    void readFile(std::string filename, MeshComponent *MeshComp, int LODlvl, CollisionComponent *CollisionComponent, MeshComponent *CollLines);
    void createCollisionCorners(std::string filename, CollisionComponent *CollisionComponent);
    void createXYZAxis(MeshComponent *MeshComp, CollisionComponent *CollisionComponent, MeshComponent *CollLines);
    void createTerrain(std::string filename, MeshComponent *MeshComp, CollisionComponent *CollisionComponent, MeshComponent *CollLines);
    void createHeightCurves(MeshComponent *MeshComp, CollisionComponent *CollisionComponent, MeshComponent *CollLines);

    void findCollisionCorners(CollisionComponent *CollComp, QVector3D &vertex);
    void findCollisionRadious(CollisionComponent *CollComp, QVector3D &vertex);
    void makeCollisionBox(CollisionComponent *CollisionComp, MeshComponent* CollisionLines);





private:
//    std::vector<GameObject*> mObjects;
//    std::map<std::string, GameObject> mObjectsMap;

    // code from VisSim
    MeshComponent* terrainMesh{nullptr};

    struct lessThanZ
    {
        inline bool operator() (gsl::Vector3D a, gsl::Vector3D b)
        {
            if(a.z < b.z) return true;
            if(a.z > b.z) return false;

            if(a.x < b.x) return true;
            if(a.x > b.x) return false;

            return false;
        }
    };

    struct lessThanX
    {
        inline bool operator() (gsl::Vector3D a, gsl::Vector3D b)
        {
            return (a.x < b.x);
        }
    };

};

#endif // MESHHANDLER_H

#ifndef MESHHANDLER_H
#define MESHHANDLER_H

#include <QOpenGLFunctions_4_1_Core>
#include "vertex.h"
#include "gltypes.h"
#include "components.h"
#include "texturehandler.h"
//#include "coreengine.h"
#include "camera.h"

struct MeshData
{
    GLuint mVAO[3]{0};
    GLuint mVBO[3]{0};
    GLuint mEAB[3]{0};
    GLint mVertexCount[3]{-1};
    GLint mIndexCount[3]{-1};
    GLenum mDrawType{GL_TRIANGLES};
    std::vector<Vertex> mVertices[3];
    std::vector<Vertex> positions;
    std::vector<GLuint> mIndices[3];
    //AABB Collider box: (the largest of these will be the radius for a Bounding sphere)
    gsl::Vector3D mUpRightFrontCorner{};
    gsl::Vector3D mLowLeftBackCorner{};
    float mColliderRadius{0};

    TransformComponent *mTransform;

};

//Because we need OpenGLFunctions, this class can not be static


class MeshHandler : public QOpenGLFunctions_4_1_Core
{
public:
    MeshHandler();

    int makeMesh(std::string meshName);

    ///Makes an AABB box matching the size of the mesh given
    MeshData makeLineBox(std::string meshName);

    ///Makes a sphere of 3 circles with given radius.
    /// Color option for rgb or pink
    MeshData makeCircleSphere(float radius = 1, bool rgbColor = true);

    MeshData makeFrustum(const struct Frustum &frustumIn);

    std::map<std::string, unsigned int> mMeshMap;
    std::vector<MeshData> mMeshes;

    std::vector<Vertex> getPositions() const;
    std::vector<Vertex> getmVertices() const;
    void setPositions(const std::vector<Vertex> &value);


    void calculateNormals();
    void initTerrain();
    //void updateParticles(const float dt);

    TextureHandler *mTexture;
    unsigned short mWidth{0};
    unsigned short mDepth{0};
    float mHorisontalSpacing{1.f};
    float mHeightSpacing{1.f};
    float mHeightPlacement{0.f};

    float vertexXStart{0.f};
    float vertexZStart{0.f};
private:
    int readObj(std::string filename);
    int readObj2(std::string filename);
    int makeAxis();
    int makeTriangle();
    int makeCube();
    int makeParticle();
    int makeProjectile();
    int makeSkybox();
    int makeTerrain(std::string heightMapName);   

    void calculateHeighMapNormals(int width, int depth, MeshData &mesh);

    //LAS SETUP
    int makeLAS(std::string fileName);
    void ReadDatafromFile(std::string fileName, MeshData &mesh);

    //void RemoveDeltaPos(MeshData &mesh);
    //void GenerateHeightMap(MeshData &mesh);
    //float CalcHeight(float x = 0, float z = 0);

    void minMaxNormalize();
    std::vector<Vertex> lasData;
    float xMin{-20.f}, yMin{0.f}, zMin{-20.f}, xMax{20.f}, yMax{6.f}, zMax{20.0f};
    float step{2.f};
    float highestX, highestY, highestZ, lowestX, lowestY, lowestZ;

    float posX = 1.f;
    float posY = 1.f;
    float posZ = 1.f;


    struct Particle
    {
        gsl::Vector3D position;
        float lifetime;
    };
    std::vector< Particle > particles;
    //float positions[400];
    float PosArr[1000][2000];

    void makeColliderCorners(MeshData &meshIn, gsl::Vector3D &vertexIn);

    ///Initializes the mesh with OpenGL buffers - VAO, VBO, EAB.
    void initMesh(MeshData &tempMesh, int lodLevel);
};

#endif // MESHHANDLER_H

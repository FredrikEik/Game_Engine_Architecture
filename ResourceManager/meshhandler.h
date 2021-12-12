#ifndef MESHHANDLER_H
#define MESHHANDLER_H

#include <QOpenGLFunctions_4_1_Core>
#include "vertex.h"
#include "gltypes.h"
#include "components.h"
#include "texturehandler.h"
//#include "coreengine.h"
#include "camera.h"
#include "gameobject.h"


struct Physics
{
    float radius = 0.25;
    float mass = 100;
    float friction = 0.9;
    float lilleG = 9.81;

    gsl::Vector3D Acceleration{0.0, -lilleG, 0};
    gsl::Vector3D Force{0.0,0.0,0.0};
    gsl::Vector3D Velocity{0,0,0};
    gsl::Vector3D VelocityOld{0,0,0};
    gsl::Vector3D airF{0,0,0};

    bool frittfall{false};

    void freeFall()
    {
        frittfall = true;
        Acceleration = gsl::Vector3D(0, -lilleG, 0);
        calculateAirF();
    }
    void onGround(gsl::Vector3D N)
    {
        if(frittfall == true)
            VelocityOld.z = 0;
        frittfall = false;
        Acceleration = gsl::Vector3D(N.x * N.z, (N.z*N.z)-1, N.y * N.z) * lilleG;
        calculateAirF();
    }
    void calculateAirF()
    {
        float p = 1.29; //mass density of air on earth
        gsl::Vector3D u{0-Velocity.x,0-Velocity.y,0-Velocity.z};//flow velocity
        float A = M_PI * (radius*radius); //Area
        float dc = 0.47; //drag coefficient
        //airF = 1/2 p * (u^2) * dc * A;
        airF = gsl::Vector3D(u.x*u.x, u.y*u.y, u.z * u.z);
        airF = airF * (0.5 * p);
        airF = airF * dc;
        airF = airF * A;

        Force = Acceleration * mass;
        Force = Force - airF;
        Acceleration = {Force.x/mass, Force.y/mass, Force.z/mass};
    }
};

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
    //MeshComponent* mMeshComponent;

};

//Because we need OpenGLFunctions, this class can not be static
class GameObject;
typedef gsl::Vector3D Vec3;
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
    void lagTriangel(const Vec3& v1, const Vec3& v2, const Vec3& v3, MeshData &tempMesh);
    void subDivide(const Vec3& a, const Vec3& b, const Vec3& c, int n, MeshData &tempMesh);
    void oktaederUnitBall(MeshData &tempMesh);
    void move(float x, float y, float z, GameObject *tempMesh);
    void moveAlongLAs( float dt, GameObject *ball);
    void baryMove(float x, float y, float z);
    void setSurface2(GameObject* surface, GameObject* ball);
    //gsl::Vector3D Get_position();
    //void setPosition(gsl::Vector3D v);
    void setHeight(float z, GameObject* ball);
    void heightAt(MeshData &tempMesh);

    gsl::Vector3D(baryCoord(const gsl::Vector2D &p1, const gsl::Vector2D &p2, const gsl::Vector2D &p3, const gsl::Vector2D &pos));
    float barycentricHeight(const gsl::Vector3D &point, const gsl::Vector3D &corner1, const gsl::Vector3D &corner2, const gsl::Vector3D &corner3);

    Physics* p;
    
    GameObject* _las;
    
    //void updateParticles(const float dt);

    TextureHandler *mTexture;
    unsigned short mWidth{0};
    unsigned short mDepth{0};
    float mHorisontalSpacing{1.f};
    float mHeightSpacing{1.f};
    float mHeightPlacement{0.f};

    float vertexXStart{0.f};
    float vertexZStart{0.f};
      //  std::vector<Vertex> surfVertices;
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
    int makeBall(int n);

    void calculateHeighMapNormals(int width, int depth, MeshData &mesh);

    //LAS SETUP
    int makeLAS(std::string fileName);
    void ReadDatafromFile(std::string fileName, MeshData &mesh);

    //void RemoveDeltaPos(MeshData &mesh);
    //void GenerateHeightMap(MeshData &mesh);
    //float CalcHeight(float x = 0, float z = 0);

    int m_rekursjoner;
    int m_indeks;               // brukes i rekursjon, til å bygge m_vertices
    gsl::Vector3D m_normal{0.0, 0.0, 1.0};
    gsl::Vector3D old_normal{0.0, 0.0, 1.0};
    gsl::Vector3D mN{0.0, 0.0, 1.0};
    int old_index{0};    

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

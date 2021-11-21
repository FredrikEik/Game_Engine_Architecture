#ifndef OCTAHEDRONBALL_H
#define OCTAHEDRONBALL_H
#include "gameobject.h"

class Vertex;
typedef gsl::Vector3D Vec3;
class OctahedronBall : public GameObject
{
private:
    int mRecursions = 0;
    int mIndex = 0;
    void makeTriangle(const Vec3 &v1, const Vec3 &v2, const Vec3 &v3);
    void subDivide(const Vec3 &v1, const Vec3 &v2, const Vec3 &v3, int n);
    void octahedronUnitBall();
public:
    OctahedronBall(int recursions);
    ~OctahedronBall();
    virtual void init() override;
    virtual void draw() override;
    virtual void move(float x, float y, float z) override;
};

#endif // OCTAHEDRONBALL_H

#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "visualobject.h"

struct MeshData;

class RollingBall : public VisualObject
{
public:
    RollingBall(int loadedMeshIndex);

    virtual void init(GLint matrixUniform) override;
    virtual void draw() override;

    virtual void init(GLint matrixUniform, const std::vector<Vertex> &vertices, const std::vector<Vertex> &indices);
    virtual void draw(const MeshData& meshData);
    void move(float dt);
    bool findTriangle(unsigned int index, QVector3D& position, QVector3D& outBaryCords, QVector3D& outP, QVector3D& outQ, QVector3D& outR) const;
    float getHeight(QVector3D& positon);
    void calculateMovement(const QVector3D& p, const QVector3D& q, const QVector3D& r);
    void doCollition();

    void setSurface(VisualObject* surface) { las_plane = surface; }

    VisualObject* getSurface() { return las_plane; }
protected:
    VisualObject* las_plane;
private:
    QVector3D old_normal{0.0, 0.0, 1.0};
    QVector3D current_normal{};
    unsigned int old_index{0};
    unsigned int current_index{0};

    int meshIndex{-1};

    QVector3D velocity{};
    QVector3D acceleration{};
    QVector3D baseForce{};
    QVector3D baseAcceleration{0.f, 0.f, -9.81f};
    const float mass{2.f};
    const float gravity{-9.81f};
    const float radius{0.25f};
};
#endif // ROLLINGBALL_H

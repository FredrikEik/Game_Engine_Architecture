#ifndef PLANE_H
#define PLANE_H

#include "visualobject.h"

class Plane : public VisualObject
{
public:
    Plane(std::string filename); //Construct the plane from the height map. (.bmp)

    virtual void init(GLint matrixUniform) override;
    virtual void draw() override;
    virtual void constructPlane();

    float getHeight(QVector3D position);
    QVector3D calculateNormal(QVector3D point3D, int index);

    int getRows() const;

protected:
    class Texture* m_heightMap{nullptr};

    float height;
    int columns;
    int rows;
    std::vector<QVector3D> tempPosition;

};

#endif // PLANE_H

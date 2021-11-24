#ifndef LASPLANE_H
#define LASPLANE_H

#include "visualobject.h"

class LasPlane : public VisualObject
{
public:
    LasPlane(std::string filename);

    virtual void init(GLint matrixUniform) override;
    virtual void draw() override;
    virtual void readFile(const std::string &filename, QVector3D &max, QVector3D &min, std::vector<QVector3D> &tempLasPos);

    void constructPlane();
    QVector3D calculateNormal(QVector3D point3D, unsigned int index, const std::vector<QVector3D> &tempPosition);

private:
    std::string filepath{};

};

#endif // LASPLANE_H

#ifndef MARCHINGSQUARE_H
#define MARCHINGSQUARE_H
#include "lasheightmap.h"
#include "gsl/vector2d.h"
#include "components.h"

class marchingsquare
{
public:
    marchingsquare(LASHeightMap* mSurf);
    ~marchingsquare(){}
    void setSquares();
    int getCase(int a, int b, int c, int d);
    void drawLine(gsl::Vector2D a, gsl::Vector2D b);
    MeshComponent * getMeshComp(){return mMesh;}
private:
    MeshComponent* mMesh;
    LASHeightMap* surface;
    int iso = 5;
    int floor = 1;
};

#endif // MARCHINGSQUARE_H

#ifndef BSPLINESYSTEM_H
#define BSPLINESYSTEM_H
#include <vertex.h>

class bsplinesystem
{
public:
    bsplinesystem(class RenderWindow * RW);
    void initialize(int entity);
    gsl::Vector3D deBoor(double x);
    int findKnotInterval(float x);
    void addC();
private:
    class RenderWindow * RW;
    int n = 50; //Antall kontrollpunkter
    int d = 2; //grad
    int seedNumber=0;
    std::vector<gsl::Vector3D> c; //Kontrollpunkter
    std::vector<double> t {0,0,0,1,2,3,4,5,6,7,7,7}; //Skjøtevektor
    std::vector<Vertex> mVertices;
    int targetEntity;
};

#endif // BSPLINESYSTEM_H

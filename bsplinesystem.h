#ifndef BSPLINESYSTEM_H
#define BSPLINESYSTEM_H
#include <vertex.h>

class bsplinesystem
{
public:
    bsplinesystem(int dID);
    void initialize();
    gsl::Vector3D deBoor(double x);
    int findKnotInterval(float x);
    void addC(std::string filnavn);
private:
    std::string mTxt = "../GEA2021/BSpline";
    int n = 9; //Antall kontrollpunkter
    int d = 2; //grad
    std::vector<gsl::Vector3D> c; //Kontrollpunkter
    std::vector<double> t {0,0,0,1,2,3,4,5,6,7,7,7}; //Skjøtevektor
    std::vector<Vertex> mVertices;
    //Posisjon etc i visualObject
};

#endif // BSPLINESYSTEM_H

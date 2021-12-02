#include "bsplinesystem.h"
#include "renderwindow.h"




bsplinesystem::bsplinesystem(RenderWindow * inRW)
{
    RW = inRW;
}

void bsplinesystem::initialize(int inEntity)
{
    targetEntity = inEntity;
    addC();
    for(double x = t[0]; x<t[n]; x+=0.05)
    {
        gsl::Vector3D punkt = deBoor(x);
        Vertex v{};
        v.set_xyz(punkt.x, punkt.y, punkt.z); v.set_rgb(1,0,0);
        mVertices.push_back(v);
    }
    seedNumber++;
    std::string objName = "BSplineOBJ"+std::to_string(targetEntity);
    RW->ResSys->SetIntoMeshDataContainerRUNTIME(mVertices,objName);
    RW->entitySys->construct(objName,QVector3D(0,0,0),0,0,-1,GL_LINE_STRIP);
    mVertices.clear();
    c.clear();
}


gsl::Vector3D bsplinesystem::deBoor(double x)
{
    int my = findKnotInterval(x);
    gsl::Vector3D a[3];
    //a.reserve(d+1);
    for(int j = 0; j<=d; j++){
        a[d-j] = c[my-j];
    }
    for(int k = d; k>0; k--){
        int j = my-k;
        for(int i=0; i<k; i++){
            j++;
            float w =(x-t[j])/(t[j+k]-t[j]);
            a[i] = a[i] * (1-w) + a[i+1]*w;
        }
    }
    return a[0];
}



int bsplinesystem::findKnotInterval(float x)
{
    int my = n-1; //Indeks til siste kontrollpunkt
    while (x<t[my] && my>d) {
        my--;
    }return my;
}


void bsplinesystem::addC()
{    
    for(int i = 0; i < (int)RW->transformCompVec.size(); i++){
        if(RW->transformCompVec[i]->entity == targetEntity){
            c=RW->transformCompVec[i]->PosOverTime;
        }
    }
    n = static_cast<int>(c.size());
}

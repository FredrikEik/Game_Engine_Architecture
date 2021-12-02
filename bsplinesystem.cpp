#include "bsplinesystem.h"
bsplinesystem::bsplinesystem(int dID)
{
    std::string sID = std::to_string(dID);
    mTxt = mTxt + sID;
    mTxt = mTxt + ".txt";
//    mMesh = new Mesh;
//    mMatrix.setToIdentity();
//    mMesh->mDrawType = GL_LINE_STRIP;
    initialize();
}

void bsplinesystem::initialize()
{
    addC(mTxt);
    for(double x = t[0]; x<t[n]; x+=0.05)
    {
        gsl::Vector3D punkt = deBoor(x);
        Vertex v{};
        v.set_xyz(punkt.x, punkt.y, punkt.z); v.set_rgb(1,0,0);
        mVertices.push_back(v);
    }
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


void bsplinesystem::addC(std::string filnavn)
{
    std::ifstream inn;
    inn.open(filnavn.c_str());

    if (inn.is_open())
    {
        int m;
        gsl::Vector3D temp;
        inn >> m;
        mVertices.reserve(m);
        for (int i=0; i<m; i++){
            inn >> temp.x;
            inn >> temp.y;
            inn >> temp.z;

            c.push_back(temp);
        }
        inn.close();
    }
    n = static_cast<int>(c.size());
}

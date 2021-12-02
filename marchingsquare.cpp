#include "marchingsquare.h"

marchingsquare::marchingsquare(LASHeightMap* mSurf)
{
    surface = mSurf;
    mMesh = new MeshComponent();
    for(int i = 0; i<mSurf->getHeighestY(); i = i+ iso)
         {setSquares();}

}

void marchingsquare::setSquares()
{
    LASHeightMap* mSurf = static_cast<LASHeightMap*>(surface);
    for(int i{0}; i<mSurf->getX(); i++){
        for(int j{0}; j<mSurf->getZ(); j++)
        {
            int rez = 1;
            float x = i;
            float z = j;
            //float z = (mSurf->mHPoints[i][j] + mSurf->mHPoints[i+1][j])/2;
            gsl::Vector2D a {x + rez*0.5f, z           };
            gsl::Vector2D b {x + rez     , z + rez*0.5f};
            gsl::Vector2D c {x + rez*0.5f, z + rez     };
            gsl::Vector2D d {x           , z + rez*0.5f};
            float aH, bH, cH, dH;
            if(mSurf->mHPoints[i][j] < iso * floor)
                aH = 0;
            else
                aH = 1;
            if(mSurf->mHPoints[i+1][j] < iso* floor)
                bH = 0;
            else
                bH = 1;
            if(mSurf->mHPoints[i+1][j+1] < iso* floor)
                cH = 0;
            else
                cH = 1;
            if(mSurf->mHPoints[i][j+1] < iso* floor)
                dH = 0;
            else
                dH = 1;


            int state = getCase(aH, bH, cH, dH);

            switch (state)
            {
            case 1:
                drawLine(c, d);
                break;
            case 2:
                drawLine(b, c);
                break;
            case 3:
                drawLine(b, d);
                break;
            case 4:
                drawLine(a, b);
                break;
            case 5:
                drawLine(a, d);
                drawLine(b, c);
                break;
            case 6:
                drawLine(a, c);
                break;
            case 7:
                drawLine(a, d);
                break;
            case 8:
                drawLine(a, d);
                break;
            case 9:
                drawLine(a, c);
                break;
            case 10:
                drawLine(a, b);
                drawLine(c, d);
                break;
            case 11:
                drawLine(a, b);
                break;
            case 12:
                drawLine(b, d);
                break;
            case 13:
                drawLine(b, c);
                break;
            case 14:
                drawLine(c, d);
                break;
            }
        }}
    floor++;
}

int marchingsquare::getCase(int a, int b, int c, int d)
{
    return a*8 + b*4 + c*2 + d*1;
}

void marchingsquare::drawLine(gsl::Vector2D a, gsl::Vector2D b)
{
    mMesh->mVertices.push_back(Vertex{a.x,static_cast<float>(iso*floor)+ 0.2f , a.y, 0,0,0, 0,0});
    mMesh->mVertices.push_back(Vertex{b.x,static_cast<float>(iso*floor)+ 0.2f , b.y, 0,0,0, 0,0});
}
